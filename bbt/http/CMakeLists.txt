#bbt_http
add_library(bbt_http
    server/serve_mux.cc
    server/connection.cc
    server/connection_manager.cc
    server/request_parser.cc
    client.cc
    server.cc
    response.cc
    request.cc
    url.cc
    form_data.cc
)
target_link_libraries(bbt_http bbt_base bbt_html)


# bbt_http_test
add_executable(bbt_http_test
    server/serve_mux_test.cc
    server_test.cc
    url_test.cc
    form_data_test.cc
    )
target_link_libraries(bbt_http_test bbt_http gmock gmock_main)
add_test(NAME bbt_http COMMAND bbt_http_test)

