# 参考：
# https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html
#

cmake_minimum_required(VERSION 3.10)
project(bbt)

# enable c++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# default build debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third_party)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 -Wall")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -fPIC -Wall -O0 -g --coverage")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 -Wall  -O0 -g --coverage")

add_definitions(-DBBT_OPTION_HARDENED=0)

include_directories(
    ${THIRD_PARTY_DIR}/googletest-1.8.1/googlemock/include
    ${THIRD_PARTY_DIR}/googletest-1.8.1/googletest/include
    ${PROJECT_SOURCE_DIR}/include
)

enable_testing()

add_subdirectory(${THIRD_PARTY_DIR}/googletest-1.8.1)
add_subdirectory(bbt)
add_subdirectory(tests)

add_custom_target(lcov
    lcov 
    -c 
    -t bbt
    -o ${CMAKE_BINARY_DIR}/bbt_lcov.info 
    -d ${PROJECT_SOURCE_DIR} 
    -b ${PROJECT_SOURCE_DIR} 
)

add_custom_target(lcov_html
    genhtml -o ${CMAKE_BINARY_DIR}/lcov_html ${CMAKE_BINARY_DIR}/bbt_lcov.info 
)