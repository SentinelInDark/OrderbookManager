//
// Created by Liam Zhang on 2022/7/3.
//

#include "StringUtils.h"

namespace obm {
    std::string_view removeRedundantSpace(std::string &s) {
        auto *pos = s.data();
        auto *tail = pos;

        while (*tail) {
            if (std::isspace(*tail) && std::isspace(*(tail - 1))) {
                ++tail;
            } else {
                *pos++ = *tail++;
            }
        }
        return {s.data(), static_cast<std::string_view::size_type>(pos - s.data())};
    }
} /// end namespace obm
