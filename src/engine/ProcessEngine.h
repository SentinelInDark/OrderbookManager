/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#ifndef ORDERBOOKMANAGER_PROCESSENGINE_H
#define ORDERBOOKMANAGER_PROCESSENGINE_H

#include <memory>
#include "../meta/Command.h"
#include "../meta/Event.h"
#include "../utils/MpscDoubleBufferQueue.h"
#include "../utils/StringUtils.h"
#include "accountbook/BuyerAccountBook.h"
#include "accountbook/SellerAccountBook.h"

namespace obm {

    class ProcessEngine {
    public:
        ProcessEngine(const std::string& , std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>>,
                      std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Event>>>);
        void run();
        void shutdown();
        void processCommand(std::shared_ptr<Command>&);
        std::shared_ptr<Order> getOrder(orderIdType);   /// For testing
    private:
        void initCommandActionMap();
        void rebuildState(const std::string &);
        void buildState(const std::shared_ptr<Event>&);
        void deliveryEvents(const std::vector<std::shared_ptr<Event>>&);

    private:
        std::atomic<bool>   m_isRunning;
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> m_commandQueue;
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Event>>> m_eventWrapperQueue;

        BuyerAccountBook m_buyerAccountBook;
        SellerAccountBook m_sellerAccountBook;

    private:
        std::unordered_map<Command::CommandType, std::function<void(std::shared_ptr<Command>)>> m_commandActionMap;
    };

}  /// end namespace obm

#endif //ORDERBOOKMANAGER_PROCESSENGINE_H
