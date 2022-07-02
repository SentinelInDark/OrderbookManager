//
// Created by Liam Zhang on 2022/7/2.
//

#ifndef ORDERBOOKMANAGER_ORDER_H
#define ORDERBOOKMANAGER_ORDER_H

#include <cstdint>
#include <string>

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
        OrderStatus     status_;
        OrderSide       side_;
        std::string     price_;
        quantityType    quantity_;
        quantityType    leaves_;
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_ORDER_H
