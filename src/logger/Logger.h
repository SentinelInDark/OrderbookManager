/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#ifndef ORDERBOOKMANAGER_LOGGER_H
#define ORDERBOOKMANAGER_LOGGER_H

#include "spdlog/spdlog.h"

namespace obm {
    void initLogger(const char*);
}  /// end namespace obm

#endif //ORDERBOOKMANAGER_LOGGER_H
