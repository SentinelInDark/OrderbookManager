//
// Created by Liam Zhang on 2022/7/3.
//

#ifndef ORDERBOOKMANAGER_PROCESSENGINE_H
#define ORDERBOOKMANAGER_PROCESSENGINE_H

#include <memory>
#include "../meta/Command.h"
#include "../meta/EventWrapper.h"
#include "../utils/MpscDoubleBufferQueue.h"
#include "accountbook/BuyerAccountBook.h"
#include "accountbook/SellerAccountBook.h"

namespace obm {

    class ProcessEngine {
    public:
        ProcessEngine(std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>>,
                      std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<EventWrapper>>>);
        void initCommandActionMap();
        void run();
        void shutdown();
    private:
        void processCommand(std::shared_ptr<Command>&);

    private:
        std::atomic<bool>   m_isRunning;
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> m_commandQueue;
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<EventWrapper>>> m_eventWrapperQueue;

        BuyerAccountBook m_buyerAccountBook;
        SellerAccountBook m_sellerAccountBook;

    private:
        std::unordered_map<Command::CommandType, std::function<void(std::shared_ptr<Command>)>> m_commandActionMap;
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_PROCESSENGINE_H
