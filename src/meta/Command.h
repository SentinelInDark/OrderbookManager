//
// Created by Liam Zhang on 2022/7/2.
//

#ifndef ORDERBOOKMANAGER_COMMAND_H
#define ORDERBOOKMANAGER_COMMAND_H

#include <memory>
#include "Order.h"

namespace obm {

    enum class CommandType {
        CREATE,
        PRINT,
        CANCEL,
        REPLACE,
    };

    class Command {
    public:
        Command(CommandType commandType, std::unique_ptr<Order> orderPtr) : command_type_(commandType), order_ptr_(std::move(orderPtr)){}
        ~Command() = default;
        Command(const Command&) = delete;
        Command(Command &&) = delete;
        Command& operator=(const Command &) = delete;
        Command& operator=(Command &&) = delete;
    private:
        CommandType             command_type_;
        std::unique_ptr<Order>  order_ptr_;
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_COMMAND_H
