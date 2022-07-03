//
// Created by Liam Zhang on 2022/7/2.
//

#ifndef ORDERBOOKMANAGER_ORDERBOOKMANAGER_H
#define ORDERBOOKMANAGER_ORDERBOOKMANAGER_H

#include <thread>
#include <memory>

#include "../utils/MpscDoubleBufferQueue.h"
#include "../meta/Command.h"
#include "../client/Client.h"

namespace obm /** orderbookmanager */{
    class OrderbookManager final {
    public:
        OrderbookManager(const std::string &storePath);
        ~OrderbookManager();

        OrderbookManager(const OrderbookManager &) = delete;
        OrderbookManager &operator=(const OrderbookManager &) = delete;
        OrderbookManager(OrderbookManager &&) = delete;
        OrderbookManager &operator=(OrderbookManager &&) = delete;

        void run();
//        void shutdown();
//
//    private:
//        void startEventStoreLoop();
//        void startOrderProcessLoop();
//        void startClientLoop();
//
    private:
        std::thread m_clientThread_;
//        std::thread mOrderProcessLoopThread;
//        std::thread mEventStoreLoopThread;
//
//        std::unique_ptr<OrderProcessor> mOrderProcessorLoop;
//        std::unique_ptr<EventStore> mEventStoreLoop;
        std::unique_ptr<Client> m_client_;
//
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> m_commandQueue_;
//        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<MatchEvent>>> mEventQueue;
//        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<MatchEvent>>> mReplyQueue;
//
        bool m_isRunning_;
//        std::string mStorePath;
    };
} /// end namespace obm

#endif //ORDERBOOKMANAGER_ORDERBOOKMANAGER_H
