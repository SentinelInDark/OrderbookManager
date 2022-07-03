//
// Created by Liam Zhang on 2022/7/2.
//
#include "Order.h"
#include <iostream>
namespace obm {

    Order::Order(orderIdType orderId, OrderSide side, priceType price, quantityType quantity)
            : m_orderId(orderId), m_status(OrderStatus::NEW), m_side(side), m_price(price), m_quantity(quantity),
              m_leaves(0), m_createTime(std::chrono::system_clock::now()) {
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
} /// end namespace obm
