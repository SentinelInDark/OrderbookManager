//
// Created by Liam Zhang on 2022/7/3.
//

#ifndef ORDERBOOKMANAGER_EVENTWAREHOUSE_H
#define ORDERBOOKMANAGER_EVENTWAREHOUSE_H

#include <fstream>
#include <memory>
#include "../meta/EventWrapper.h"
#include "../utils/MpscDoubleBufferQueue.h"

namespace obm {

    class EventWarehouse {
    public:
        EventWarehouse(const std::string &, std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<EventWrapper>>>);
        ~EventWarehouse();
        EventWarehouse();
        EventWarehouse(const EventWarehouse &) = delete;
        EventWarehouse(EventWarehouse &&) = delete;
        EventWarehouse& operator=(const EventWarehouse &) = delete;
        EventWarehouse& operator=(EventWarehouse &&) = delete;

        void run();
        void shutdown();
    private:
        void doWork();
        void persist(const std::shared_ptr<EventWrapper>&);
    private:
        std::fstream        m_file;
        std::atomic<bool>   m_isRunning;
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<EventWrapper>>> m_eventWrapperQueue;
    };

} // obm

#endif //ORDERBOOKMANAGER_EVENTWAREHOUSE_H
