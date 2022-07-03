//
// Created by Liam Zhang on 2022/7/3.
//
#include <iostream>
#include "ProcessEngine.h"
#include "../logger/Logger.h"

namespace obm {
    ProcessEngine::ProcessEngine(std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> cmdQueue,
                                 std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<EventWrapper>>> evtQueue)
            : m_isRunning_(false), m_commandQueue_(std::move(cmdQueue)),
              m_eventWrapperQueue_(std::move(evtQueue)) {
    }

    void ProcessEngine::run() {
        if (m_isRunning_) {
            SPDLOG_INFO("ProcessEngine is running");
            return;
        }
        m_isRunning_ = true;
        while (true) {
            if (!m_isRunning_) {
                SPDLOG_WARN("ProcessEngine is shutting down");
                break;
            }
            auto cmd = m_commandQueue_->dequeue();
            if (cmd) {
                processCommand(cmd);
            }
        }
    }

    void ProcessEngine::processCommand(std::shared_ptr<Command>& cmd) {
        std::cout<<"receive "<<cmd->toString()<<std::endl;
    }

    void ProcessEngine::shutdown() {
        m_isRunning_ = false;
        SPDLOG_WARN("Call ProcessEngine::shutdown to shutdown process engine");
    }
} /// end namespace obm
