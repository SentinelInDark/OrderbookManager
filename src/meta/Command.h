//
// Created by Liam Zhang on 2022/7/2.
//

#ifndef ORDERBOOKMANAGER_COMMAND_H
#define ORDERBOOKMANAGER_COMMAND_H

#include <unordered_map>
#include <memory>
#include "Order.h"

namespace obm {

    class Command {
    public:
        enum class CommandType {
            CREATE,
            PRINT,
            CANCEL,
            REPLACE,
        };
    public:
        Command(CommandType commandType, std::unique_ptr<Order> orderPtr) : command_type_(commandType), order_ptr_(std::move(orderPtr)){}
        ~Command() = default;
        Command(const Command&) = delete;
        Command(Command &&) = delete;
        Command& operator=(const Command &) = delete;
        Command& operator=(Command &&) = delete;
        std::string toString() const;
    public:
        static std::shared_ptr<Command> buildFromStr(const std::string_view&);
    private:
        static bool isPrintCommand(const std::string_view&);
        static bool isTransactionCommand(const std::string_view&);
    private:
        CommandType             command_type_;
        std::unique_ptr<Order>  order_ptr_;

        inline static const std::string PRINT_BOOK_STR = "print book";

        inline static const std::unordered_map<std::string_view, CommandType> cmdTypeMap{
            {"new", CommandType::CREATE},
            {"cancel", CommandType::CANCEL},
            {"replace", CommandType::REPLACE}
        };

        inline static const std::unordered_map<std::string_view, obm::Order::OrderSide> orderSideMap{
                {"buy", obm::Order::OrderSide::BUY},
                {"sell", obm::Order::OrderSide::SELL}
        };
    };
} /// end namespace obm

#endif //ORDERBOOKMANAGER_COMMAND_H
