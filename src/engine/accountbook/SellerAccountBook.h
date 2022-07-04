/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

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
        bool canTrade(const std::shared_ptr<Order>&, const std::shared_ptr<Order>&) const override;
    };

}  /// end namespace obm

#endif // ORDERBOOKMANAGER_SELLERACCOUNTBOOK_H
