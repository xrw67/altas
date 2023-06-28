# 参考：
# https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html
#

cmake_minimum_required(VERSION 3.10)

project(bbt)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_BUILD_TYPE "Debug")

set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third_party)

# enable c++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Common
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wall -Werror -g -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 -Wall -Werror -g -O2")

include_directories(${PROJECT_SOURCE_DIR}/include)

# bbt_base
aux_source_directory(src/base  DIR_SRCS)

# bbt_appkit

#
# Testing
#
include(GoogleTest)
add_subdirectory(${THIRD_PARTY_DIR}/googletest-1.8.1)
enable_testing()

add_executable(bbt_test ${DIR_SRCS} src/test/hello_test.cc)
target_link_libraries(bbt_test gmock gmock_main)

gtest_discover_tests(bbt_test)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(CodeCoverage)
    append_coverage_compiler_flags()

    setup_target_for_coverage_lcov(
        NAME bbt_coverage
        EXECUTABLE make test -j ${n_cores}
        DEPENDENCIES bbt_test
    )

    set(COVERAGE_EXCLUDES 
        "build/*"
        "src/test/*"
        "third_party/*"
    )
endif()