add_library(cppboot_net
    buffer.cc
    tcp/client.cc
    tcp/server.cc
    tcp/connection.cc
    tcp/connection_manager.cc
)
target_link_libraries(cppboot_net cppboot_base)

add_executable(cppboot_net_test
    buffer_test.cc
    tcp/server_test.cc
)
target_link_libraries(cppboot_net_test cppboot_net gmock gmock_main)
add_test(NAME cppboot_net COMMAND cppboot_net_test)

