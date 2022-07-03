//
// Created by Liam Zhang on 2022/7/3.
//

#ifndef ORDERBOOKMANAGER_PROCESSENGINE_H
#define ORDERBOOKMANAGER_PROCESSENGINE_H

#include <memory>
#include "../meta/Command.h"
#include "../meta/EventWrapper.h"
#include "../utils/MpscDoubleBufferQueue.h"

namespace obm {

    class ProcessEngine {
    public:
        ProcessEngine(std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>>,
                      std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<EventWrapper>>>);
        void run();
        void shutdown();
    private:
        void processCommand(std::shared_ptr<Command>&);
    private:
        std::atomic<bool>   m_isRunning_;
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> m_commandQueue_;
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<EventWrapper>>> m_eventWrapperQueue_;
    };

} // obm

#endif //ORDERBOOKMANAGER_PROCESSENGINE_H
