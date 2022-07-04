/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#include <cstring>

#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "Logger.h"

namespace obm {
    void initLogger(const char* file_name) {
        spdlog::flush_on(spdlog::level::info);
        spdlog::set_pattern("[%D %H:%M:%S.%F] [%s:%#] [%l] [thread %t] %v");

        if (std::strcmp("stdout", file_name) == 0) {
            spdlog::stdout_logger_mt(file_name);
        } else {
            constexpr size_t FILE_SIZE = 1024 * 1024 * 1024;    // max file size is 1GB
            constexpr size_t MAX_FILE_COUNT = 100;
            const char* LOGGER_NAME = "file_logger";
            spdlog::set_default_logger(spdlog::rotating_logger_mt(LOGGER_NAME, file_name, FILE_SIZE, MAX_FILE_COUNT, true));
        }
    }
}  /// end namespace obm
