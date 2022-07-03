//
// Created by Liam Zhang on 2022/7/3.
//

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
} /// end namespace obm
