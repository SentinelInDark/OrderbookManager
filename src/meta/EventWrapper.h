//
// Created by Liam Zhang on 2022/7/3.
//

#ifndef ORDERBOOKMANAGER_EVENTWRAPPER_H
#define ORDERBOOKMANAGER_EVENTWRAPPER_H

#include <memory>
#include "Command.h"
#include "Event.h"

namespace obm {

    class EventWrapper {
    private:
        std::unique_ptr<Command>    m_originCommand_;
        std::unique_ptr<Event>      m_event_;
    };

} // obm

#endif //ORDERBOOKMANAGER_EVENTWRAPPER_H
