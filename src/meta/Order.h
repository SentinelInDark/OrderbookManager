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

    enum class OrderStatus : uint64_t {
        NEW,
        PARTIALLY_FILLED,
        FULLY_FILLED,
        CANCELED,
        REJECTED,
    };

    enum class OrderSide :uint64_t {
        BUY,
        SELL,
    };

    struct Order {
        Order(orderIdType, OrderSide, priceType, quantityType);
        void printSummary() const;
        [[nodiscard]] bool isBuyerOrder() const;
        [[nodiscard]] bool isSellerOrder() const;
        [[nodiscard]] bool canBeReplaced() const;
        [[nodiscard]] bool isFullyFilled() const;
        [[nodiscard]] std::string getStatusStr() const;
        [[nodiscard]] std::string getSideStr() const;
        [[nodiscard]] std::string toString() const;
        void decreaseQuantity(quantityType qty);

        orderIdType     m_orderId;
        OrderStatus     m_status;
        OrderSide       m_side;
        priceType       m_price;
        quantityType    m_quantity;
        std::chrono::system_clock::time_point   m_createTime;
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_ORDER_H
