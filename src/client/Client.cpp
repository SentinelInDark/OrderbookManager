//
// Created by Liam Zhang on 2022/7/3.
//
#include <iostream>
#include "Client.h"
#include "../logger/Logger.h"

#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"

namespace obm {
    Client::Client(std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> q):m_isRunning_(false), m_commandQueue_(std::move(q)) {
    }

    void Client::shutdown() {
        if (m_isRunning_) {
            m_isRunning_ = false;
            SPDLOG_INFO("Client thread is shutting down");
        }
    }

    void Client::run() {
        std::string inputCommandStr;
        while (getline(std::cin, inputCommandStr)) {
            auto commandStr_v = absl::StripTrailingAsciiWhitespace(inputCommandStr);
            if (commandStr_v.empty()) {
                continue;
            }
            auto cmd = buildCommand(commandStr_v);
            if (cmd) {
                m_commandQueue_->enqueue(cmd);
            }
        }
    }

    std::shared_ptr<Command> Client::buildCommand(const std::string_view &inputStr) {
        return nullptr;
    }

    bool Client::validateCommand(const std::string&) const {
        return true;
    }

    void Client::printUsage() {
        std::cout
                <<"usage: [New|Cancel|Replace] [orderId] [buy|sell] [quantity@price]\n"
                <<"\nEx:\n"
                <<"=====================================================\n"
                <<"\tNew 1 buy 2@100\t\tNew 1 sell 2@100\n"
                <<"\tCancel 1 buy 2@100\t\tCancel 1 sell 2@100\n"
                <<"\tReplace 1 buy 2@100\t\tCancel 1 sell 2@100"
                <<"====================================================="
                <<std::endl;
    }
} /// end namespace obm
