/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#ifndef ORDERBOOKMANAGER_STRINGUTILS_H
#define ORDERBOOKMANAGER_STRINGUTILS_H

#include <vector>
#include <string>
#include <string_view>

namespace obm {

std::string_view removeRedundantSpaceAndToLowerCase(std::string &);
std::vector<std::string_view> split(std::string_view sv, char delim = ' ');

}  ///  end namespace obm

#endif // ORDERBOOKMANAGER_STRINGUTILS_H
