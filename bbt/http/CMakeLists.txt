#bbt_http
add_library(bbt_http
    client.cc
    server.cc
    connection.cc
    connection_manager.cc
    request_parser.cc
    request_handler.cc
    response.cc
    request.cc
    url.cc
)
target_link_libraries(bbt_http bbt_base)


# bbt_http_test
add_executable(bbt_http_test
    server_test.cc
    url_test.cc
    request_handler_test.cc
    )
target_link_libraries(bbt_http_test bbt_http gmock gmock_main)
add_test(NAME bbt_http COMMAND bbt_http_test)

