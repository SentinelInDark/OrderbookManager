//
// Created by Liam Zhang on 2022/7/2.
//
#include "Order.h"
#include "absl/strings/str_format.h"
#include <iostream>

namespace obm {

    Order::Order(orderIdType orderId, OrderSide side, priceType price, quantityType quantity)
            : m_orderId(orderId), m_status(OrderStatus::NEW), m_side(side), m_price(price), m_quantity(quantity),
              m_createTime(std::chrono::system_clock::now()) {
    }

    void Order::printSummary() const {
        std::cout << this->m_quantity << '@' << this->m_price;
    }

    bool Order::isBuyerOrder() const {
        return Order::OrderSide::BUY == this->m_side;
    }

    bool Order::isSellerOrder() const {
        return Order::OrderSide::SELL == this->m_side;
    }

    bool Order::canBeReplaced() const {
        return Order::OrderStatus::NEW == this->m_status;
    }

    bool Order::isFullyFilled() const {
        return this->m_quantity == 0;
    }

    void Order::decreaseQuantity(quantityType qty) {
        if (qty == 0) {
            return;
        }
        this->m_quantity -= qty;
        if (m_quantity > 0) {
            this->m_status = OrderStatus::PARTIALLY_FILLED;
        } else if (m_quantity == 0) {
            this->m_status = OrderStatus::FULLY_FILLED;
        }
    }

    std::string Order::getStatusStr() const {
        switch (m_status) {
            case OrderStatus::NEW:
                return "NEW";
            case OrderStatus::PARTIALLY_FILLED:
                return "PARTIALLY_FILLED";
            case OrderStatus::FULLY_FILLED:
                return "FULLY_FILLED";
            case OrderStatus::CANCELED:
                return "CANCELED";
            case OrderStatus::REJECTED:
                return "REJECTED";
        }
        return "Unknown";
    }

    std::string Order::getSideStr() const {
        switch (m_side) {
            case OrderSide::SELL:
                return "SELL";
            case OrderSide::BUY:
                return "BUY";
        }
        return "Unknown";
    }

    std::string Order::toString() const {
        return absl::StrFormat("[orderId:%ld, side:%s, status:%s, price:%ld, quantity:%ld]",
                               m_orderId, getSideStr(), getStatusStr(), m_price, m_quantity);
    }
} /// end namespace obm
