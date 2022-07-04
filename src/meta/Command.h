/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#ifndef ORDERBOOKMANAGER_COMMAND_H
#define ORDERBOOKMANAGER_COMMAND_H

#include <unordered_map>
#include <memory>
#include "Order.h"

namespace obm {

    class Command {
    public:
        enum class CommandType {
            NEW,
            PRINT,
            CANCEL,
            REPLACE,
        };
    public:
        Command(CommandType commandType, std::shared_ptr<Order> orderPtr);
        ~Command();
        Command(const Command &) = delete;
        Command(Command &&) = delete;
        Command &operator=(const Command &) = delete;
        Command &operator=(Command &&) = delete;

        [[nodiscard]] std::string toString() const;
        [[nodiscard]] bool isBuyerCommand() const;
        [[nodiscard]] bool isSellerCommand() const;
        [[nodiscard]] std::shared_ptr<Order> getOrder() const;
        [[nodiscard]] CommandType getCommandType() const;

    public:
        static std::shared_ptr<Command> buildFromStr(const std::string_view &);

    private:
        inline static bool isPrintCommand(const std::string_view &);

    private:
        CommandType m_commandType;
        std::shared_ptr<Order> m_orderPtr;

        inline static const std::string PRINT_BOOK_STR = "print book";

        inline static const std::unordered_map<std::string_view, CommandType> cmdTypeMap{
                {"new",     CommandType::NEW},
                {"cancel",  CommandType::CANCEL},
                {"replace", CommandType::REPLACE}
        };

        inline static const std::unordered_map<std::string_view, obm::OrderSide> orderSideMap{
                {"buy",  obm::OrderSide::BUY},
                {"sell", obm::OrderSide::SELL}
        };
    };
} /// end namespace obm

#endif //ORDERBOOKMANAGER_COMMAND_H
