//
// Created by Liam Zhang on 2022/7/2.
//
#include "Order.h"

namespace obm {

    Order::Order(orderIdType orderId, OrderSide side, priceType price, quantityType quantity)
            : m_orderId_(orderId), m_status_(OrderStatus::NEW), m_side_(side), m_price_(price), m_quantity_(quantity),
              m_leaves_(0), m_createTime_(std::chrono::system_clock::now()) {
    }

} /// end namespace obm
