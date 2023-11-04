set(CPPBOOT_IO_SRCS "")

if(LINUX)
    append(CPPBOOT_IO_SRCS iocp.cc)
elseif(APPLE)
    append(CPPBOOT_IO_SRCS kqueue.cc)
elseif(WIN32)
    append(CPPBOOT_IO_SRCS iocp.cc)

add_library(cppboot_io ${CPPBOOT_IO_SRCS})
target_link_libraries(cppboot_io cppboot_base)

add_executable(cppboot_io_test
    context_test.cpp
)

target_link_libraries(cppboot_io_test cppboot_io gmock gmock_main)
add_test(NAME cppboot_io COMMAND cppboot_io_test)