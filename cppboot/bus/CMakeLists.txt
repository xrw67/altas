add_library(cppboot_bus
    client.cc
    server.cc
    msg.cc
    msg_packer.cc
    context.cc
    invoker.cc
    service.cc
    router.cc
)
target_link_libraries(cppboot_bus cppboot_net cppboot_base)

add_executable(cppboot_bus_test
    client_test.cc
    server_test.cc
    msg_test.cc
    msg_packer_test.cc
    invoker_test.cc
    service_test.cc
    router_test.cc
)
target_link_libraries(cppboot_bus_test cppboot_bus gmock gmock_main)
add_test(NAME cppboot_bus COMMAND cppboot_bus_test)

