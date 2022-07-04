project(tests)

# Sources
file(GLOB_RECURSE SRC_LIST "*.cpp" "*.h" "*.hpp" "*.cc")

set(LIBRARY ${ABSL_LIBS})

# unit-test on googletest
include_directories(BEFORE ../third_party/gtest/googletest)
include_directories(BEFORE ../third_party/gtest/googletest/include)
# add gmock
include_directories(BEFORE ../third_party/gtest/googlemock)
include_directories(BEFORE ../third_party/gtest/googlemock/include)

# Executable
set(SRC_LIST
        ${SRC_LIST}
        ../src/utils/MpscDoubleBufferQueue.h
        ../src/utils/StringUtils.cpp
        ../src/utils/CommonUtils.cpp
        ../src/meta/Order.cpp
        ../src/meta/Event.cpp
        ../src/meta/Command.cpp
        ../src/engine/ProcessEngine.cpp
        ../src/engine/accountbook/AbstractBaseBook.cpp
        ../src/engine/accountbook/BookKey.cpp
        ../src/engine/accountbook/BuyerAccountBook.cpp
        ../src/engine/accountbook/SellerAccountBook.cpp
        ../src/storage/EventWarehouse.cpp)

add_executable(TestRunner
        ${SRC_LIST}
        ../third_party/gtest/googlemock/src/gmock-all.cc
        ../third_party/gtest/googletest/src/gtest-all.cc)
target_link_libraries(TestRunner ${LIBRARY} )
