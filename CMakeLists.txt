# 参考：
# https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html
#

cmake_minimum_required(VERSION 3.10)

project(bbt)

set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third_party)

# enable c++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTest
include(GoogleTest)
add_subdirectory(${THIRD_PARTY_DIR}/googletest)
enable_testing()



# Common
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g f-O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g -O2")
include_directories(${PROJECT_SOURCE_DIR}/include)

# hello
add_executable(hello 
    src/hello.cc
)

# bbt_base
aux_source_directory(src/base  DIR_SRCS)

# bbt_appkit

# bbt_test
add_executable(bbt_test ${DIR_SRCS} src/test/hello_test.cc)
target_link_libraries(bbt_test gmock gmock_main)

gtest_discover_tests(bbt_test)