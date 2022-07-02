//
// Created by Liam Zhang on 2022/7/3.
//

#ifndef ORDERBOOKMANAGER_CLIENT_H
#define ORDERBOOKMANAGER_CLIENT_H

#include <memory>
#include "../meta/Command.h"
#include "../utils/MpscDoubleBufferQueue.h"

namespace obm {

    class Client {
    public:
        Client(std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>>);
        ~Client() = default;

        static void printUsage() ;
        void run();
        void shutdown();
        std::shared_ptr<Command> buildCommand(const std::string &);

    private:
        bool validateCommand(const std::string&) const;
    private:
        std::atomic<bool> is_running_;
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> command_queue_;
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_CLIENT_H
