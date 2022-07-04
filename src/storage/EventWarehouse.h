/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#ifndef ORDERBOOKMANAGER_EVENTWAREHOUSE_H
#define ORDERBOOKMANAGER_EVENTWAREHOUSE_H

#include <fstream>
#include <memory>
#incluce <string>
#include "../meta/Event.h"
#include "../utils/MpscDoubleBufferQueue.h"

namespace obm {

    class EventWarehouse {
    public:
        EventWarehouse(const std::string &, std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Event>>>);
        ~EventWarehouse();
        EventWarehouse();
        EventWarehouse(const EventWarehouse &) = delete;
        EventWarehouse(EventWarehouse &&) = delete;
        EventWarehouse& operator=(const EventWarehouse &) = delete;
        EventWarehouse& operator=(EventWarehouse &&) = delete;

        void run();
        void shutdown();
        void persist(const std::shared_ptr<Event>&);
    private:
        void doWork();
    private:
        std::atomic<bool>   m_isRunning;
        std::string         m_filePath;
        std::fstream        m_file;
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Event>>> m_eventWrapperQueue;
    };

}  ///  end namespace obm

#endif // ORDERBOOKMANAGER_EVENTWAREHOUSE_H
