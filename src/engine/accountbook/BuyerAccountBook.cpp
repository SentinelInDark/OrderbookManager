//
// Created by Liam Zhang on 2022/7/3.
//

#include <iostream>
#include "BuyerAccountBook.h"

namespace obm {
    BuyerAccountBook::BuyerAccountBook() : AbstractBaseBook(
            std::make_unique<std::map<BookKey, std::shared_ptr<Order>, BookKeyComparator>>(BookKeyComparator(true))) {

    }

    void BuyerAccountBook::print() const {
        std::cout<<"bid:";
        AbstractBaseBook::print();
        std::cout<<std::endl<<"> ";
        std::cout.flush();
    }
} /// end namespace obm