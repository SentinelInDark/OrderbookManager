//
// Created by Liam Zhang on 2022/7/3.
//
#include <iostream>
#include <fstream>
#include <utility>
#include "ProcessEngine.h"
#include "../logger/Logger.h"
#include "../utils/CommonUtils.h"

namespace obm {
    ProcessEngine::ProcessEngine(const std::string& evtDataPath, std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> cmdQueue,
                                 std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Event>>> evtQueue)
            : m_isRunning(false), m_commandQueue(std::move(cmdQueue)),
              m_eventWrapperQueue(std::move(evtQueue)) {
        initCommandActionMap();
        rebuildState(evtDataPath);
    }

    void ProcessEngine::rebuildState(const std::string & eventDataPath) {
        std::ifstream inFile(eventDataPath);
        if (!inFile.is_open()) {
            SPDLOG_WARN("Data file {} can not be opened", eventDataPath);
            return;
        }
        std::string eventStr;

        while (getline(inFile, eventStr)) {
            std::shared_ptr<Event> eventPtr = Event::decode(eventStr);
            buildState(eventPtr);
        }
        inFile.close();
    }

    void ProcessEngine::buildState(const std::shared_ptr<Event>& eventPtr) {
        if (eventPtr->m_side == OrderSide::BUY) {
            m_buyerAccountBook.restore(eventPtr);
        } else {
            m_sellerAccountBook.restore(eventPtr);
        }
    }

    void ProcessEngine::initCommandActionMap() {
        /// PRINT
        this->m_commandActionMap[Command::CommandType::PRINT] = [this](const std::shared_ptr<Command>&){
            SPDLOG_INFO("Print book command");
            m_sellerAccountBook.print();
            m_buyerAccountBook.print();
            printPrompt();
        };

        /// NEW
        this->m_commandActionMap[Command::CommandType::NEW] = [this](const std::shared_ptr<Command>& cmd){
            auto newOrderPtr = cmd->getOrder();
            SPDLOG_INFO("new order command: {}", newOrderPtr->toString());
            std::vector<std::shared_ptr<Event>> events;
            if (cmd->isBuyerCommand()) {
                m_buyerAccountBook.add(newOrderPtr, &events);
                m_sellerAccountBook.trade(newOrderPtr, &events);
                m_buyerAccountBook.cleanupOrder(newOrderPtr);
            } else if (cmd->isSellerCommand()) {
                m_sellerAccountBook.add(newOrderPtr, &events);
                m_buyerAccountBook.trade(newOrderPtr, &events);
                m_sellerAccountBook.cleanupOrder(newOrderPtr);
            } else {
                assert(0);
            }
            printPrompt();
            deliveryEvents(events);
        };

        /// CANCEL
        this->m_commandActionMap[Command::CommandType::CANCEL] = [this](const std::shared_ptr<Command>& cmd){
            auto newOrderPtr = cmd->getOrder();
            SPDLOG_INFO("cancel order command: {}", newOrderPtr->toString());
            std::vector<std::shared_ptr<Event>> events;
            if (cmd->isBuyerCommand()) {
                m_buyerAccountBook.cancel(newOrderPtr, &events);
            } else if (cmd->isSellerCommand()) {
                m_sellerAccountBook.cancel(newOrderPtr, &events);
            } else {
                assert(0);
            }
            printPrompt();
            deliveryEvents(events);
        };

        /// REPLACE
        /// TODO - replace order on other side (buy order -> sell order, sell order -> buy order)
        this->m_commandActionMap[Command::CommandType::REPLACE] = [this](const std::shared_ptr<Command>& cmd){
            auto newOrderPtr = cmd->getOrder();
            SPDLOG_INFO("replace order command: {}", newOrderPtr->toString());
            std::vector<std::shared_ptr<Event>> events;
            if (cmd->isBuyerCommand()) {
                m_buyerAccountBook.replace(newOrderPtr, &events);
            } else if (cmd->isSellerCommand()) {
                m_sellerAccountBook.replace(newOrderPtr, &events);
            } else {
                assert(0);
            }
            printPrompt();
            deliveryEvents(events);
        };
    }

    void ProcessEngine::run() {
        if (m_isRunning) {
            SPDLOG_INFO("ProcessEngine is running");
            return;
        }
        m_isRunning = true;
        while (true) {
            if (!m_isRunning) {
                SPDLOG_WARN("ProcessEngine is shutting down");
                break;
            }
            auto cmd = m_commandQueue->dequeue();
            if (cmd) {
                processCommand(cmd);
            }
        }
    }

    void ProcessEngine::deliveryEvents(const std::vector<std::shared_ptr<Event>>& events) {
        for (auto &event : events) {
            this->m_eventWrapperQueue->enqueue(event);
        }
    }

    void ProcessEngine::processCommand(std::shared_ptr<Command>& cmd) {
        this->m_commandActionMap[cmd->getCommandType()](cmd);
    }

    void ProcessEngine::shutdown() {
        m_isRunning = false;
        SPDLOG_WARN("ProcessEngine is shutting down");
    }
} /// end namespace obm
