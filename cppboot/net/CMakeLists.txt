add_library(cppboot_net
    buffer.cc
    tcp/client.cc
    tcp/server.cc
    tcp/connection.cc
    tcp/connection_manager.cc
    http/server/serve_mux.cc
    http/server/connection.cc
    http/server/connection_manager.cc
    http/server/request_parser.cc
    http/server/file_server.cc
    http/client.cc
    http/server.cc
    http/response.cc
    http/request.cc
    http/url.cc
    http/form_data.cc
    html/document.cc
    html/elements/element.cc
    html/elements/table.cc
)
target_link_libraries(cppboot_net cppboot_base)

add_executable(cppboot_net_test
    buffer_test.cc
    tcp/server_test.cc
    http/server/serve_mux_test.cc
    http/server/file_server_test.cc
    http/server_test.cc
    http/url_test.cc
    http/form_data_test.cc
    html/html_test.cc
)
target_link_libraries(cppboot_net_test cppboot_net gmock gmock_main)
add_test(NAME cppboot_net COMMAND cppboot_net_test)

add_executable(file_server http/server/file_server_demo.cc)
target_link_libraries(file_server cppboot_net)