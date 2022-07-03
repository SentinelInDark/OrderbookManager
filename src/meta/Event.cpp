//
// Created by Liam Zhang on 2022/7/3.
//

#include "absl/strings/numbers.h"
#include "absl/strings/str_format.h"
#include "Event.h"
#include "../utils/StringUtils.h"

namespace obm {
    Event::Event(orderIdType orderId, OrderStatus status, OrderSide side, priceType price,
                 quantityType quantity, quantityType filledQuantity)
            : m_orderId(orderId), m_status(status), m_side(side), m_price(price), m_quantity(quantity), m_filledQuantity(filledQuantity) {

    }

    std::string Event::encode() const {
        return absl::StrFormat("%ld|%ld|%ld|%ld|%ld|%ld\n", m_orderId, static_cast<uint64_t>(m_status), static_cast<uint64_t>(m_side),
                               m_price, m_quantity, m_filledQuantity);
    }

    std::shared_ptr<Event> Event::decode(const std::string& eventStr) {
        auto sv = split(eventStr, '|');
        decltype(sv.size()) index = 0;
        orderIdType orderId;
        if (!absl::SimpleAtoi(sv[index++], &orderId)) {
            return nullptr;
        }
        std::underlying_type<OrderStatus>::type status = 0;
        if (!absl::SimpleAtoi(sv[index++], &status)) {
            return nullptr;
        }

        std::underlying_type<OrderSide>::type side = 0;
        if (!absl::SimpleAtoi(sv[index++], &side)) {
            return nullptr;
        }
        priceType price;
        if (!absl::SimpleAtoi(sv[index++], &price)) {
            return nullptr;
        }
        quantityType quantity;
        if (!absl::SimpleAtoi(sv[index++], &quantity)) {
            return nullptr;
        }
        quantityType filledQuantity;
        if (!absl::SimpleAtoi(sv[index++], &filledQuantity)) {
            return nullptr;
        }

        return std::make_shared<Event>(orderId, static_cast<OrderStatus>(status), static_cast<OrderSide>(side), price, quantity, filledQuantity);
    }
} /// end namespace obm
