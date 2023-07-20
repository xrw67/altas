add_library(bbt_net 
    buffer.cc
    ipaddr.cc
    http/request.cc
    http/response.cc
    http/server.cc
)
target_link_libraries(bbt_net bbt_util)

add_executable(bbt_net_test
    buffer_test.cc
    ipaddr_test.cc
    http/request_test.cc
    http/response_test.cc
    http/server_test.cc
    )
target_link_libraries(bbt_net_test bbt_net gmock gmock_main)
add_test(NAME bbt_net COMMAND bbt_net_test)