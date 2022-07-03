//
// Created by Liam Zhang on 2022/7/3.
//

#include "EventWarehouse.h"
#include "../logger/Logger.h"

namespace obm {
    EventWarehouse::EventWarehouse(
            const std::string &filePath, std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Event>>> queue) :
            m_isRunning(false), m_eventWrapperQueue(std::move(queue)) {
        m_file.open(filePath.data(), std::ios::in | std::ios::out | std::ios::app);
    }

    EventWarehouse::~EventWarehouse() {
        if (m_file.is_open()) {
            m_file.close();
        }
    }

    void EventWarehouse::shutdown() {
        if (m_isRunning) {
            SPDLOG_INFO("EventWarehouse is shutting down");
            m_isRunning = false;
        }
    }

    void EventWarehouse::run() {
        if (m_isRunning) {
            SPDLOG_WARN("EventWarehouse has already been running");
            return;
        }
        m_isRunning = true;
        doWork();
    }

    void EventWarehouse::doWork() {
        while (true) {
            if (!m_isRunning) {
                break;
            }
            auto evt = m_eventWrapperQueue->dequeue();
            if (evt) {
                persist(evt);
            }
        }
    }

    void EventWarehouse::persist(const std::shared_ptr<Event>& eventPtr) {

    }
} /// end namespace obm
