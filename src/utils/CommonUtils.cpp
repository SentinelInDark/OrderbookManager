/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#include <iostream>
#include "CommonUtils.h"

namespace obm {
    void printPrompt() {
        std::cout<<"> ";
        std::cout.flush();
    }
} /// end namespace obm
