#pragma once

// for cmake
// 0.1.0
#define ORDERBOOK_MANAGER_VER_MAJOR 0
#define ORDERBOOK_MANAGER_VER_MINOR 1
#define ORDERBOOK_MANAGER_VER_PATCH 0

// for source code
// v0.1.0
#define _ORDERBOOK_MANAGER_STR(s) #s
#define ORDERBOOK_MANAGER_PROJECT_VERSION(major, minor, patch) "v" _ORDERBOOK_MANAGER_STR(major.minor.patch)