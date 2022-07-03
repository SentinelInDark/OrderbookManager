//
// Created by Liam Zhang on 2022/7/3.
//

#ifndef ORDERBOOKMANAGER_EVENT_H
#define ORDERBOOKMANAGER_EVENT_H

#include "Order.h"

namespace obm {

    struct Event {
        orderIdType         m_orderId;
        Order::OrderStatus  m_status;
        Order::OrderSide    m_side;
        priceType           m_price;
        quantityType        m_quantity;
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_EVENT_H
