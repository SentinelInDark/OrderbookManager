/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#ifndef ORDERBOOKMANAGER_BOOKKEY_H
#define ORDERBOOKMANAGER_BOOKKEY_H

#include "../../meta/Order.h"
#include <chrono>
#include <map>
#include <memory>

namespace obm {

    struct BookKey {
        BookKey(orderIdType orderId, priceType price, std::chrono::system_clock::time_point createTime);

        obm::orderIdType orderId{};
        obm::priceType price{};
        std::chrono::system_clock::time_point createTime;
    };

    class BookKeyComparator {
    public:
        explicit BookKeyComparator(bool mIsBuyer);
        bool operator()(const BookKey &, const BookKey &) const;
    private:
        bool m_isBuyerBook;
    };

}  /// end namespace obm

#endif // ORDERBOOKMANAGER_BOOKKEY_H
