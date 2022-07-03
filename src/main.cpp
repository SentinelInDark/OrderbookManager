//
// Created by Liam Zhang on 2022/7/2.
//
#include <unistd.h>
#include "logger/Logger.h"
#include "app/OrderbookManager.h"

int main(int argc, char *argv[]) {
    const char *default_args[] = {"stdout", "/tmp/data"};
    obm::initLogger(argc > 1 ? argv[1] : default_args[0]);

    SPDLOG_INFO("OrderbookManager is running, pid={}", getpid());

    auto dataPath = (argc > 2 ? argv[2] : default_args[1]);
    obm::OrderbookManager(dataPath).run();
    return 0;
}
