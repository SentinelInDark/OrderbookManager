/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#include "OrderbookManager.h"
#include "../logger/Logger.h"

namespace obm {
    OrderbookManager::OrderbookManager(const std::string &storePath) : m_isRunning(false) {
        m_commandQueue = std::make_shared<MpscDoubleBufferQueue<std::shared_ptr<Command>>>();
        m_eventQueue = std::make_shared<MpscDoubleBufferQueue<std::shared_ptr<Event>>>();
        m_client = std::make_unique<Client>(m_commandQueue);
        m_processEngine = std::make_unique<ProcessEngine>(storePath, m_commandQueue, m_eventQueue);
        m_eventWarehouse = std::make_unique<EventWarehouse>(storePath, m_eventQueue);
    }

    OrderbookManager::~OrderbookManager() = default;

    void OrderbookManager::run() {
        if (m_isRunning) {
            SPDLOG_WARN("OrderbookManager has already been running");
            return;
        }
        m_isRunning = true;
        SPDLOG_INFO("OrderbookManager is running");

        /// start event warehouse engine thread
        this->m_eventWarehouseThread = std::thread([this]() {
            pthread_setname_np("EventWarehouse");
            this->m_eventWarehouse->run();
        });

        /// start process engine thread
        this->m_commandProcessThread = std::thread([this]() {
            pthread_setname_np("ProcessThread");
            this->m_processEngine->run();
        });

        /// start client thread
        this->m_clientThread = std::thread([this]() {
            pthread_setname_np("ClientThread");
            this->m_client->run();
        });

        m_clientThread.join();
        m_commandProcessThread.join();
        m_eventWarehouseThread.join();
    }

    void OrderbookManager::shutdown() const {
        m_client->shutdown();
        if (!m_commandQueue->empty()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        m_processEngine->shutdown();
        if (!m_eventQueue->empty()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        m_eventWarehouse->shutdown();
    }
} /// end namespace obm
