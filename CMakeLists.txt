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

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Common
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 -Wall")


add_definitions(-DBBT_OPTION_HARDENED=0)

include_directories(${PROJECT_SOURCE_DIR}/include)

if(BBT_BUILD_TESTS)
    enable_testing()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

    # Use GoogleTest
    include(GoogleTest)
    add_subdirectory(${THIRD_PARTY_DIR}/googletest-1.8.1)

    # Code Coverage
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()

add_subdirectory(base)
add_subdirectory(appkit)
add_subdirectory(modules)
add_subdirectory(test)

if(BBT_BUILD_TESTS)
    setup_target_for_coverage_lcov(
        NAME coverage
        EXECUTABLE ctest -j ${n_cores}
        DEPENDENCIES test_bbt test_base test_appkit test_mod_debugger
        EXCLUDE "third_party/*" "*_test.cc" "/usr/include/*"
    )

endif()