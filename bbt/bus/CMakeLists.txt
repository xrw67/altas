add_library(bbt_bus
    client.cc
    server.cc
    msg.cc
    msg_packer.cc
    msg_queue.cc
    server/connection.cc
    server/connection_manager.cc
)
target_link_libraries(bbt_bus bbt_base)

add_executable(bbt_bus_test
    bus_test.cc
    msg_test.cc
    msg_packer_test.cc
    msg_queue_test.cc
)
target_link_libraries(bbt_bus_test bbt_bus gmock gmock_main)
add_test(NAME bbt_bus COMMAND bbt_bus_test)

