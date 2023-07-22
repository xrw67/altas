add_library(bbt_io 
    buffer.cc
    ipaddr.cc
    http/request.cc
    http/response.cc
    http/server.cc
)
target_link_libraries(bbt_io bbt_base)

add_executable(bbt_io_test
    buffer_test.cc
    ipaddr_test.cc
    http/request_test.cc
    http/response_test.cc
    http/server_test.cc
    )
target_link_libraries(bbt_io_test bbt_io gmock gmock_main)
add_test(NAME bbt_io COMMAND bbt_io_test)