#cppboot_http
add_library(cppboot_http
    server/serve_mux.cc
    server/connection.cc
    server/connection_manager.cc
    server/request_parser.cc
    server/file_server.cc
    client.cc
    server.cc
    response.cc
    request.cc
    url.cc
    form_data.cc
)
target_link_libraries(cppboot_http cppboot_base cppboot_html)


# cppboot_http_test
add_executable(cppboot_http_test
    server/serve_mux_test.cc
    server/file_server_test.cc
    server_test.cc
    url_test.cc
    form_data_test.cc
    )
target_link_libraries(cppboot_http_test cppboot_http gmock gmock_main)
add_test(NAME cppboot_http COMMAND cppboot_http_test)

add_executable(file_server server/file_server_demo.cc)
target_link_libraries(file_server cppboot_base cppboot_http)