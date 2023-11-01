set(BBT_IO_SRCS "")

if(LINUX)
    append(BBT_IO_SRCS iocp.cc)
elseif(APPLE)
    append(BBT_IO_SRCS kqueue.cc)
elseif(WIN32)
    append(BBT_IO_SRCS iocp.cc)

add_library(bbt_io ${BBT_IO_SRCS})
target_link_libraries(bbt_io bbt_base)

add_executable(bbt_io_test
    context_test.cpp
)

target_link_libraries(bbt_io_test bbt_io gmock gmock_main)
add_test(NAME bbt_io COMMAND bbt_io_test)