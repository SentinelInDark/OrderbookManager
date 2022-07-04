/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#ifndef ORDERBOOKMANAGER_CLIENT_H
#define ORDERBOOKMANAGER_CLIENT_H

#include <memory>
#include "../meta/Command.h"
#include "../utils/MpscDoubleBufferQueue.h"

namespace obm {

    class Client {
    public:
        explicit Client(std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>>);
        ~Client() = default;

        static void printUsage() ;
        void run();
        void shutdown();

    private:
        void processCommand() const;
        [[nodiscard]] std::shared_ptr<Command> buildCommand(const std::string_view &) const;
    private:
        std::atomic<bool> m_isRunning;
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> m_commandQueue;
    };

} /// end namespace obm

#endif //ORDERBOOKMANAGER_CLIENT_H
