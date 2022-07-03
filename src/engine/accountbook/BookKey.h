//
// Created by Liam Zhang on 2022/7/3.
//

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

} // obm

#endif //ORDERBOOKMANAGER_BOOKKEY_H
