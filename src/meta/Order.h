//
// Created by Liam Zhang on 2022/7/2.
//

#ifndef ORDERBOOKMANAGER_ORDER_H
#define ORDERBOOKMANAGER_ORDER_H

#include <memory>
#include <vector>
#include <cstdint>
#include <string>
#include <chrono>

namespace obm {
    using orderIdType = uint64_t;
    using quantityType = uint64_t;
    using priceType = uint64_t; /// TODO: price is BigDecimal

    struct Order {
        enum class OrderStatus {
            NEW,
            PARTIALLY_FILLED,
            FULLY_FILLED,
            CANCELED,
            REJECTED,
        };

        enum class OrderSide {
            BUY,
            SELL,
        };
        Order(orderIdType, OrderSide, priceType, quantityType);

        orderIdType     m_orderId_;
        OrderStatus     m_status_;
        OrderSide       m_side_;
        priceType       m_price_;
        quantityType    m_quantity_;
        quantityType    m_leaves_;
        std::chrono::system_clock::time_point   m_createTime_;
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_ORDER_H
