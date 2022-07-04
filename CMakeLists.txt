cmake_minimum_required(VERSION 3.22)

#--------------------------------------------
# Project setting
#--------------------------------------------
set(CMAKE_C_COMPILER "gcc-11")
set(CMAKE_CXX_COMPILER "g++-11")

include(cmake/utils.cmake)
orderbook_manager_extract_version()

project(OrderBookManager VERSION ${ORDERBOOK_MANAGER_VERSION} LANGUAGES CXX)

message(STATUS "Build OrderBookManager with version: ${ORDERBOOK_MANAGER_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "compiler is: " ${CMAKE_CXX_COMPILER_ID})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-overflow -fPIC")

cmake_policy(SET CMP0114 NEW)

# output dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# include dir
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSPDLOG_NO_EXCEPTIONS=ON")
include_directories(third_party/spdlog/include)
include_directories(third_party/abseil-cpp)

# link dir
link_directories(${CMAKE_BINARY_DIR}/build)
# static link libgcc and libstdc++
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# add absl lib
option(ABSL_PROPAGATE_CXX_STD "ABSL_PROPAGATE_CXX_STD" ON)
add_subdirectory(third_party/abseil-cpp)
set(ABSL_LIBS absl::strings absl::base absl::str_format)

set(FLAG_LIST
        -Wall
        -Wextra
        -Wno-error=unused-but-set-variable
        -Wcast-align
        -Wcast-qual
        -Wconversion
        #-Weffc++
        -Wformat=2
        #-Wmissing-format-attribute
        -Wno-non-template-friend
        #-Wno-unused-parameter
        -Woverloaded-virtual
        -Wwrite-strings
        )
foreach (FLAG ${FLAG_LIST})
    set(CMAKE_SELF_CODE_CXX_FLAGS "${CMAKE_SELF_CODE_CXX_FLAGS} ${FLAG}")
endforeach ()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_SELF_CODE_CXX_FLAGS}")
#set(CMAKE_CXX_COMPILER "g++-11")

add_subdirectory(src)
add_subdirectory(tests)

file(GLOB_RECURSE SRC_LST "src/*.cpp" "src/*.h" "src/*.hpp" "src/*.cc")

# custom targets
add_custom_target(check COMMAND python3 ../third_party/cpplint/cpplint.py --recursive ${SRC_LST} 2>&1 > /tmp/cpplint.out)
