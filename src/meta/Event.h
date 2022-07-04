/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#ifndef ORDERBOOKMANAGER_EVENT_H
#define ORDERBOOKMANAGER_EVENT_H

#include "Order.h"

namespace obm {

    struct Event {
        Event(orderIdType, OrderStatus, OrderSide, priceType, quantityType, quantityType);
        [[nodiscard]] std::string encode() const;
        static std::shared_ptr<Event> decode(const std::string&);

        orderIdType     m_orderId;
        OrderStatus     m_status;
        OrderSide       m_side;
        priceType       m_price;
        quantityType    m_quantity;
        quantityType    m_filledQuantity;
    };

}  /// end namespace obm

#endif // ORDERBOOKMANAGER_EVENT_H
