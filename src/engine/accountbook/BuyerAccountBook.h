//
// Created by Liam Zhang on 2022/7/3.
//

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
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_BUYERACCOUNTBOOK_H
