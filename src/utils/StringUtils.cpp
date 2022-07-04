/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#include "StringUtils.h"

namespace obm {
    std::string_view removeRedundantSpaceAndToLowerCase(std::string &s) {
        auto *pos = s.data();
        auto *tail = pos;

        while (*tail) {
            if (std::isspace(*tail) && std::isspace(*(tail - 1))) {
                ++tail;
            } else {
                *pos++ = static_cast<char>(std::tolower(*tail++));
            }
        }
        return {s.data(), static_cast<std::string_view::size_type>(pos - s.data())};
    }

    std::vector<std::string_view> split(std::string_view sv, char delim) {
        std::vector<std::string_view> output;
        size_t first = 0;
        auto size = sv.size();

        while (first < size) {
            const auto second = sv.find_first_of(delim, first);
            if (first != second) {
                output.emplace_back(sv.substr(first, second - first));
            }
            if (second == std::string_view::npos) {
                break;
            }
            first = second + 1;
        }
        return output;
    }
}  ///  end namespace obm
