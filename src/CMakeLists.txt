cmake_minimum_required(VERSION 3.22)
project(OrderBookManager)

file(GLOB_RECURSE SRC_FILE_LIST
        ${PROJECT_SOURCE_DIR}/*.h
        ${PROJECT_SOURCE_DIR}/*.hpp
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/*.cc)

#file(GLOB_RECURSE SRC_FILE_LIST "*.cpp" "*.h" "*.hpp" "*.cc")

add_executable(OrderBookManager ${SRC_FILE_LIST})
