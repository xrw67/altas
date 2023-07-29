# bbt_io
add_library(bbt_io 
    buffer.cc
    ipaddr.cc
)
target_link_libraries(bbt_io bbt_base)


# bbt_io_test
add_executable(bbt_io_test
    buffer_test.cc
    ipaddr_test.cc
    )
target_link_libraries(bbt_io_test bbt_io gmock gmock_main)
add_test(NAME bbt_io COMMAND bbt_io_test)