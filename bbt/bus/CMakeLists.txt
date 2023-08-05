add_library(bbt_bus
    client.cc
    server.cc
)
target_link_libraries(bbt_bus bbt_base)

add_executable(bbt_bus_test
    bus_test.cc
)
target_link_libraries(bbt_bus_test bbt_bus gmock gmock_main)
add_test(NAME bbt_bus COMMAND bbt_bus_test)

