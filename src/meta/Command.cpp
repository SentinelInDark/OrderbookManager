/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#include "Command.h"

#include <utility>
#include "../utils/StringUtils.h"
#include "absl/strings/numbers.h"

namespace obm {

    Command::Command(CommandType commandType, std::shared_ptr<Order> orderPtr) : m_commandType(commandType), m_orderPtr(std::move(orderPtr)){

    }

    Command::~Command() = default;

    /// TODO: promote price type from integer to big decimal
    std::shared_ptr<Command> Command::buildFromStr(const std::string_view& sv) {
        if (isPrintCommand(sv)) {
            return std::make_shared<Command>(CommandType::PRINT, nullptr);
        }
        auto cmdVector = obm::split(sv, ' ');
        /// sample command: New 4 sell 2@100 or Cancel 4 sell 100
        if (cmdVector.size() != 4) {
            return nullptr;
        }

        auto cmdTypeIter = cmdTypeMap.find(cmdVector[0]);
        if (cmdTypeIter == cmdTypeMap.end()) {
            return nullptr;
        }

        orderIdType orderId;
        if (!absl::SimpleAtoi(cmdVector[1], &orderId)) {
            return nullptr;
        }

        auto sideIter = orderSideMap.find(cmdVector[2]);
        if (sideIter == orderSideMap.end()) {
            return nullptr;
        }

        auto cmdType = cmdTypeIter->second;
        if (cmdType == CommandType::CANCEL) {
            priceType price;
            if (!absl::SimpleAtoi(cmdVector[3], &price)) {
                return nullptr;
            }
            return std::make_shared<Command>(cmdType, std::make_shared<Order>(orderId, sideIter->second, price, 0));
        } else {
            auto qtyPriceVector = obm::split(cmdVector[3], '@');
            if (qtyPriceVector.size() != 2) {
                return nullptr;
            }
            quantityType quantity;
            if (!absl::SimpleAtoi(qtyPriceVector[0], &quantity)) {
                return nullptr;
            }
            priceType price;
            if (!absl::SimpleAtoi(qtyPriceVector[1], &price)) {
                return nullptr;
            }
            return std::make_shared<Command>(cmdType, std::make_shared<Order>(orderId, sideIter->second, price, quantity));
        }
    }

    inline bool Command::isPrintCommand(const std::string_view& sv) {
        return sv == PRINT_BOOK_STR;
    }

    bool Command::isBuyerCommand() const {
        return m_orderPtr && m_orderPtr->isBuyerOrder();
    }

    bool Command::isSellerCommand() const {
        return m_orderPtr && m_orderPtr->isSellerOrder();
    }

    std::shared_ptr<Order> Command::getOrder() const {
        return m_orderPtr;
    }

    Command::CommandType Command::getCommandType() const {
        return m_commandType;
    }

    std::string Command::toString() const {
        if (m_commandType == CommandType::NEW) {
            return "NEW";
        }
        if (m_commandType == CommandType::CANCEL) {
            return "CANCEL";
        }
        if (m_commandType == CommandType::PRINT) {
            return "print book";
        }
        if (m_commandType == CommandType::REPLACE) {
            return "REPLACE";
        }
        return "unknown";
    }
} /// end namespace obm