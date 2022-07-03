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
    using priceType = uint64_t; /// TODO: price type is BigDecimal

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
        void printSummary() const;
        [[nodiscard]] bool isBuyerOrder() const;
        [[nodiscard]] bool isSellerOrder() const;
        [[nodiscard]] bool canBeReplaced() const;
        [[nodiscard]] std::string getStatusStr() const;

        orderIdType     m_orderId;
        OrderStatus     m_status;
        OrderSide       m_side;
        priceType       m_price;
        quantityType    m_quantity;
        quantityType    m_leaves;
        std::chrono::system_clock::time_point   m_createTime;
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_ORDER_H
