//
// Created by Liam Zhang on 2022/7/3.
//
#include <iostream>
#include "ProcessEngine.h"
#include "../logger/Logger.h"
#include "../utils/CommonUtils.h"

namespace obm {
    ProcessEngine::ProcessEngine(std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> cmdQueue,
                                 std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Event>>> evtQueue)
            : m_isRunning(false), m_commandQueue(std::move(cmdQueue)),
              m_eventWrapperQueue(std::move(evtQueue)) {
        initCommandActionMap();
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
            deliveryEvents(events);
        };

        /// CANCEL
        this->m_commandActionMap[Command::CommandType::CANCEL] = [this](const std::shared_ptr<Command>& cmd){
            //TODO
        };

        /// REPLACE
        this->m_commandActionMap[Command::CommandType::REPLACE] = [this](const std::shared_ptr<Command>& cmd){
            auto orderPtr = cmd->getOrder();
            bool isBuyerOrder = false;
            auto ptr = m_buyerAccountBook.find(orderPtr);
            if (ptr) {
                isBuyerOrder = true;
            } else {
                ptr = m_sellerAccountBook.find(orderPtr);
            }

            if (!ptr) {
                std::cout<<"Order "<<cmd->getOrder()->m_orderId<<" not found"<<std::endl;
            } else {
                if (ptr->canBeReplaced()) {
                    if (isBuyerOrder) {
                        m_buyerAccountBook.remove(orderPtr);
                    } else {
                        m_sellerAccountBook.remove(orderPtr);
                    }
                    m_commandActionMap[Command::CommandType::NEW](cmd);
                } else {
                    std::cout<<"Order "<<cmd->getOrder()->m_orderId<<"'s status is "<<orderPtr->getStatusStr()<<" can not be replaced"<<std::endl;
                }
            }
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
