/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#ifndef ORDERBOOKMANAGER_BUYERACCOUNTBOOK_H
#define ORDERBOOKMANAGER_BUYERACCOUNTBOOK_H

#include "AbstractBaseBook.h"

namespace obm {

    class BuyerAccountBook : public AbstractBaseBook {
    public:
        BuyerAccountBook();
        ~BuyerAccountBook() override = default;

        BuyerAccountBook(const BuyerAccountBook &) = delete;
        BuyerAccountBook(BuyerAccountBook &&) = delete;
        BuyerAccountBook& operator=(const BuyerAccountBook &) = delete;
        BuyerAccountBook& operator==(BuyerAccountBook &&) = delete;

        void print() const override;
        bool canTrade(const std::shared_ptr<Order>&, const std::shared_ptr<Order>&) const override;
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_BUYERACCOUNTBOOK_H
