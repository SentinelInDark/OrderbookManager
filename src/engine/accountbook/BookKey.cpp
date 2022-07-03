//
// Created by Liam Zhang on 2022/7/3.
//

#include "BookKey.h"

namespace obm {
    bool BookKeyComparator::operator()(const BookKey &key1, const BookKey &key2) const {
        if (key1.price == key2.price) {
            /// TODO: consider to compare by order create time
            return key1.orderId > key2.orderId;
        }
        if (key1.price > key2.price) {
            return m_isBuyerBook;
        }
        return !m_isBuyerBook;
    }

    BookKeyComparator::BookKeyComparator(bool isBuyer) : m_isBuyerBook(isBuyer) {}

    BookKey::BookKey(orderIdType orderId, priceType price, std::chrono::system_clock::time_point createTime) : orderId(
            orderId), price(price), createTime(createTime) {}
} /// end namespace obm