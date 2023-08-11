add_library(bbt_net
    buffer.cc
    tcp_client.cc
    tcp_server.cc
    tcp_connection.cc
    tcp_connection_manager.cc
)
target_link_libraries(bbt_net bbt_base)

add_executable(bbt_net_test
    buffer_test.cc
    tcp_server_test.cc
)
target_link_libraries(bbt_net_test bbt_net gmock gmock_main)
add_test(NAME bbt_net COMMAND bbt_net_test)

