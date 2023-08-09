add_library(bbt_bus
    client.cc
    server.cc
    msg.cc
    msg_packer.cc
    method_mux.cc
    context.cc
    connection.cc
)
target_link_libraries(bbt_bus bbt_net bbt_base)

add_executable(bbt_bus_test
    bus_test.cc
    msg_test.cc
    msg_packer_test.cc
    method_mux_test.cc
    connection_test.cc
)
target_link_libraries(bbt_bus_test bbt_bus gmock gmock_main)
add_test(NAME bbt_bus COMMAND bbt_bus_test)

