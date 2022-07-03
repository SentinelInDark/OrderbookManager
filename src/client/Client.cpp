//
// Created by Liam Zhang on 2022/7/3.
//
#include <iostream>
#include "Client.h"
#include "../logger/Logger.h"
#include "../utils/StringUtils.h"
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
        this->m_isRunning_ = true;
        SPDLOG_INFO("Client thread is running.");
        printUsage();
        processCommand();
    }

    void Client::processCommand() const {
        printPrompt();

        std::string inputCommandStr;
        while (getline(std::cin, inputCommandStr)) {
            auto command_sv = absl::StripTrailingAsciiWhitespace(removeRedundantSpaceAndToLowerCase(inputCommandStr));
            if (command_sv.empty()) {
                continue;
            }
            auto cmd = buildCommand(command_sv);
            if (cmd) {
                m_commandQueue_->enqueue(cmd);
            } else {
                std::cout<<"Bad command: "<<inputCommandStr<<std::endl;
            }
            printPrompt();
        }
    }

    void Client::printPrompt() {
        std::cout<<"> ";
    }

    std::shared_ptr<Command> Client::buildCommand(const std::string_view &inputStr) const {
        return Command::buildFromStr(inputStr);
    }

    void Client::printUsage() {
        std::cout
                <<"\nusage: \n"
                <<"transaction command: [New|Cancel|Replace] [orderId] [buy|sell] [quantity@price]\n"
                <<"query command: print book\n"
                <<"Ex:\n"
                <<"=================================================================\n"
                <<"\tNew 1 buy 2@100   \t\tNew 1 sell 2@100\n"
                <<"\tCancel 1 buy 200  \t\tCancel 1 sell 200\n"
                <<"\tReplace 1 buy 2@100\t\tReplace 1 sell 2@100\n"
                <<"\tprint book\n"
                <<"================================================================="
                <<std::endl;
    }
} /// end namespace obm
