//
// Created by Liam Zhang on 2022/7/2.
//

#include "OrderbookManager.h"
#include "../logger/Logger.h"

namespace obm {
    OrderbookManager::OrderbookManager(const std::string &storePath) : m_isRunning_(false) {
        m_commandQueue_ = std::make_shared<MpscDoubleBufferQueue<std::shared_ptr<Command>>>();
        mEventWrapperQueue_ = std::make_shared<MpscDoubleBufferQueue<std::shared_ptr<EventWrapper>>>();
        m_client_ = std::make_unique<Client>(m_commandQueue_);
        m_processEngine_ = std::make_unique<ProcessEngine>(m_commandQueue_, mEventWrapperQueue_);
    }

    OrderbookManager::~OrderbookManager() = default;

    void OrderbookManager::run() {
        if (m_isRunning_) {
            SPDLOG_WARN("OrderbookManager has already been running");
            return;
        }
        m_isRunning_ = true;
        SPDLOG_INFO("OrderbookManager is running");

        /// start client thread
        this->m_clientThread_ = std::thread([this]() {
            pthread_setname_np("ClientThread");
            this->m_client_->run();
        });

        /// start process engine thread
        this->mCommandProcessThread_ = std::thread([this]() {
            pthread_setname_np("ProcessThread");
            this->m_processEngine_->run();
        });
        m_clientThread_.join();
        mCommandProcessThread_.join();
    }

    void OrderbookManager::shutdown() const {
        m_client_->shutdown();
        m_processEngine_->shutdown();
    }
} /// end namespace obm
