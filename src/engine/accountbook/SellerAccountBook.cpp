/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#include <iostream>
#include "SellerAccountBook.h"

namespace obm {
    SellerAccountBook::SellerAccountBook() : AbstractBaseBook(
            std::make_unique<std::map<BookKey, std::shared_ptr<Order>, BookKeyComparator>>(BookKeyComparator(false))) {

    }

    void SellerAccountBook::print() const {
        std::cout<<"ask:";
        AbstractBaseBook::print();
    }

    bool SellerAccountBook::canTrade(const std::shared_ptr<Order>& orderPtr, const std::shared_ptr<Order>& newOrderPtr) const {
        return orderPtr && orderPtr->m_price <= newOrderPtr->m_price;
    }
}  /// end namespace obm
