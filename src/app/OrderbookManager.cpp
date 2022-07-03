//
// Created by Liam Zhang on 2022/7/2.
//

#include "OrderbookManager.h"
#include "../logger/Logger.h"

namespace obm {
    OrderbookManager::OrderbookManager(const std::string &storePath):m_isRunning_(false) {
        m_commandQueue_ = std::make_shared<MpscDoubleBufferQueue<std::shared_ptr<Command>>>();
        m_client_ = std::make_unique<Client>(m_commandQueue_);
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
        this->m_clientThread_ = std::thread([this](){
            pthread_setname_np("ClientThread");
            this->m_client_->run();
        });
        m_clientThread_.join();
    }

} /// end namespace obm
