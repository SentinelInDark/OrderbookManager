//
// Created by Liam Zhang on 2022/7/2.
//

#ifndef ORDERBOOKMANAGER_ORDER_H
#define ORDERBOOKMANAGER_ORDER_H

#include <cstdint>
#include <string>
#include <chrono>

namespace obm {
    using quantityType = uint64_t;

    enum class OrderStatus {
        NEW,
        PARTIALLY_FILLED,
        FULLY_FILLED,
        CANCELED,
        REJECTED,
    };

    enum class OrderSide {
        BUYER,
        SELLER,
    };

    class Order {
    private:
        OrderStatus     m_status_;
        OrderSide       m_side_;
        std::string     m_price_;
        quantityType    m_quantity_;
        quantityType    m_leaves_;
        std::chrono::system_clock::time_point   m_createTime;
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_ORDER_H
