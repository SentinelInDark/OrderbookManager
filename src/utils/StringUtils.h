//
// Created by Liam Zhang on 2022/7/3.
//

#ifndef ORDERBOOKMANAGER_STRINGUTILS_H
#define ORDERBOOKMANAGER_STRINGUTILS_H

#include <vector>
#include <string>
#include <string_view>

namespace obm {

std::string_view removeRedundantSpaceAndToLowerCase(std::string &);
std::vector<std::string_view> split(std::string_view sv, char delim = ' ');

} // obm

#endif //ORDERBOOKMANAGER_STRINGUTILS_H
