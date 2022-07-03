//
// Created by Liam Zhang on 2022/7/3.
//

#ifndef ORDERBOOKMANAGER_SELLERACCOUNTBOOK_H
#define ORDERBOOKMANAGER_SELLERACCOUNTBOOK_H

#include "AbstractBaseBook.h"

namespace obm {

    class SellerAccountBook : public AbstractBaseBook {
    public:
        SellerAccountBook();

        ~SellerAccountBook() override = default;

        SellerAccountBook(const SellerAccountBook &) = delete;

        SellerAccountBook(SellerAccountBook &&) = delete;

        SellerAccountBook &operator=(const SellerAccountBook &) = delete;

        SellerAccountBook &operator==(SellerAccountBook &&) = delete;

        void print() const override;
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_SELLERACCOUNTBOOK_H
