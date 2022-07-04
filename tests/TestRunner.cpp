/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#include "gtest/gtest.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    // ::testing::UnitTest &unitTest = *::testing::UnitTest::GetInstance();
    // ::testing::TestEventListeners &listeners = unitTest.listeners();
    spdlog::stdout_logger_mt("console");
    spdlog::set_pattern("[%D %H:%M:%S.%F] [%s:%# %!] [%l] [thread %t] %v");
    // add listeners here
    int ret = RUN_ALL_TESTS();
    return ret;
}