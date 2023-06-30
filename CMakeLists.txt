# 参考：
# https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html
#

cmake_minimum_required(VERSION 3.10)
project(bbt)

# enable c++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BBT_BUILD_TESTS "Build unit tests" ON)
option(BBT_BUILD_BENCHMARKS "Build benchmarks" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_BUILD_TYPE "Debug")

set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third_party)



# Common
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 -Wall -Werror")


add_definitions(-DBBT_OPTION_HARDENED=0)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Base
file(GLOB_RECURSE BBT_BASE_SRCS "src/base/*.cc")
list(FILTER BBT_BASE_SRCS EXCLUDE REGEX ".*_test.cc$") # Except unit test sources
add_library(bbt_base SHARED ${BBT_BASE_SRCS})

# Appkit
file(GLOB_RECURSE BBT_APPKIT_SRCS "src/appkit/*.cc")
list(FILTER BBT_APPKIT_SRCS EXCLUDE REGEX ".*_test.cc$") # Except unit test sources
add_library(bbt_appkit SHARED ${BBT_APPKIT_SRCS})
target_link_libraries(bbt_appkit bbt_base)


if(BBT_BUILD_TESTS)
    enable_testing()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

    # Use GoogleTest
    include(GoogleTest)
    add_subdirectory(${THIRD_PARTY_DIR}/googletest-1.8.1)

    file(GLOB_RECURSE BBT_TESTS_SRCS "src/*_test.cc")
    add_executable(tests ${BBT_TESTS_SRCS})
    target_link_libraries(tests bbt_base bbt_appkit gmock gmock_main)
    gtest_discover_tests(tests)

    # Code Coverage
    include(CodeCoverage)
    append_coverage_compiler_flags()

    setup_target_for_coverage_lcov(
        NAME coverage
        EXECUTABLE make test -j ${n_cores}
        DEPENDENCIES tests
        EXCLUDE "thrid_party/*" "*_test.cc"
    )

endif()
