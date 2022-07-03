//
// Created by Liam Zhang on 2022/7/2.
//
#include "Command.h"
#include "../utils/StringUtils.h"
#include "absl/strings/numbers.h"

namespace obm {

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
            return std::make_shared<Command>(cmdType, std::make_unique<Order>(orderId, sideIter->second, price, 0));
        } else {
            auto qtyPriceVector = obm::split(cmdVector[3], '@');
            if (qtyPriceVector.size() != 2) {
                return nullptr;
            }
            priceType price;
            if (!absl::SimpleAtoi(qtyPriceVector[0], &price)) {
                return nullptr;
            }
            quantityType quantity;
            if (!absl::SimpleAtoi(qtyPriceVector[1], &quantity)) {
                return nullptr;
            }
            return std::make_shared<Command>(cmdType, std::make_unique<Order>(orderId, sideIter->second, price, quantity));
        }
    }

    bool Command::isPrintCommand(const std::string_view& sv) {
        return sv == PRINT_BOOK_STR;
    }

    bool Command::isTransactionCommand(const std::string_view& sv) {
        return !isPrintCommand(sv);
    }

    std::string Command::toString() const {
        return "";
    }
} /// end namespace obm