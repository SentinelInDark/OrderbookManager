/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#include <iostream>
#include "BuyerAccountBook.h"

namespace obm {
    BuyerAccountBook::BuyerAccountBook() : AbstractBaseBook(
            std::make_unique<std::map<BookKey, std::shared_ptr<Order>, BookKeyComparator>>(BookKeyComparator(true))) {

    }

    void BuyerAccountBook::print() const {
        std::cout<<"bid:";
        AbstractBaseBook::print();
    }

    bool BuyerAccountBook::canTrade(const std::shared_ptr<Order>& orderPtr, const std::shared_ptr<Order>& newOrderPtr) const {
        return orderPtr && orderPtr->m_price >= newOrderPtr->m_price;
    }
}  /// end namespace obm
