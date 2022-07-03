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
        std::cout<<"bid:";
    }
} /// end namespace obm