add_library(bbt_bus
    client.cc
    server.cc
    msg.cc
    msg_packer.cc
    context.cc
    invoker.cc
    service.cc
    router.cc
)
target_link_libraries(bbt_bus bbt_net bbt_base)

add_executable(bbt_bus_test
    client_test.cc
    server_test.cc
    bus_test.cc
    msg_test.cc
    msg_packer_test.cc
    invoker_test.cc
    service_test.cc
    router_test.cc
)
target_link_libraries(bbt_bus_test bbt_bus gmock gmock_main)
add_test(NAME bbt_bus COMMAND bbt_bus_test)

