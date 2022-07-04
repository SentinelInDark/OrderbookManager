/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#ifndef ORDERBOOKMANAGER_ORDERBOOKMANAGER_H
#define ORDERBOOKMANAGER_ORDERBOOKMANAGER_H

#include <thread>
#include <memory>
#include <string>

#include "../utils/MpscDoubleBufferQueue.h"
#include "../meta/Command.h"
#include "../client/Client.h"
#include "../engine/ProcessEngine.h"
#include "../storage/EventWarehouse.h"

namespace obm /** orderbookmanager */{
    class OrderbookManager final {
    public:
        explicit OrderbookManager(const std::string &storePath);
        ~OrderbookManager();

        OrderbookManager(const OrderbookManager &) = delete;
        OrderbookManager &operator=(const OrderbookManager &) = delete;
        OrderbookManager(OrderbookManager &&) = delete;
        OrderbookManager &operator=(OrderbookManager &&) = delete;

        void run();
        void shutdown() const;
    private:
        std::thread m_clientThread;
        std::thread m_commandProcessThread;
        std::thread m_eventWarehouseThread;

        std::unique_ptr<EventWarehouse> m_eventWarehouse;
        std::unique_ptr<Client> m_client;
        std::unique_ptr<ProcessEngine> m_processEngine;

        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> m_commandQueue;
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Event>>> m_eventQueue;

        bool m_isRunning;
        std::string m_dataStorePath;
    };
}  /// end namespace obm

#endif // ORDERBOOKMANAGER_ORDERBOOKMANAGER_H
