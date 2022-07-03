//
// Created by Liam Zhang on 2022/7/2.
//
#include <iostream>
#include "Command.h"
#include "absl/strings/str_split.h"

namespace obm {

    std::shared_ptr<Command> Command::buildFromStr(const std::string_view& sv) {
        if (isPrintCommand(sv)) {
            return std::make_shared<Command>(CommandType::PRINT, nullptr);
        }

        auto cmdSegments = absl::StrSplit(sv, ' ');
        for (auto &x : cmdSegments) {
            std::cout<<x<<std::endl;
        }
        return nullptr;
    }

    bool Command::isPrintCommand(const std::string_view& sv) {
        return sv == "print book";
    }

    bool Command::isTransactionCommand(const std::string_view& sv) {
        return !isPrintCommand(sv);
    }

    std::string Command::toString() const {
        return "";
    }
} /// end namespace obm