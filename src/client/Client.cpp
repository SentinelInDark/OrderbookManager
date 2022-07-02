//
// Created by Liam Zhang on 2022/7/3.
//
#include <iostream>
#include "Client.h"
#include "../logger/logger.h"

namespace obm {
    Client::Client(std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> q):is_running_(false), command_queue_(std::move(q)) {
    }

    void Client::shutdown() {
        if (is_running_) {
            is_running_ = false;
            SPDLOG_INFO("Client thread is shutting down");
        }
    }

    void Client::run() {
        std::string inputStr;
        while (getline(std::cin, inputStr)) {
            if (inputStr.empty()) {
                continue;
            }
            //cmd = absl::StripTrailingAsciiWhitespace(cmd);
            auto cmd = buildCommand(inputStr);
            if (cmd) {
                this->command_queue_->enqueue(cmd);
            }
        }
    }

    std::shared_ptr<Command> Client::buildCommand(const std::string &inputStr) {
        return nullptr;
    }

    bool Client::validateCommand(const std::string&) const {
        return true;
    }

    void Client::printUsage() {
        std::cout
                <<"usage: [New|Cancel|Replace] [orderId] [buy|sell] [quantity@price]\n"
                <<"\nex:\n"
                <<"=====================================================\n"
                <<"\tNew 1 buy 2@100\t\tNew 1 sell 2@100\n"
                <<"\tCancel 1 buy 2@100\t\tCancel 1 sell 2@100\n"
                <<"\tReplace 1 buy 2@100\t\tCancel 1 sell 2@100"
                <<"====================================================="
                <<std::endl;
    }
} /// end namespace obm
