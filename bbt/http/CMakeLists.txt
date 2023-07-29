#bbt_http
add_library(bbt_http
    server.cc
    connection.cc
    connection_manager.cc
    request_parser.cc
    request_handler.cc
    reply.cc
)
target_link_libraries(bbt_http bbt_base)

# bbt_http_server_example
add_executable(bbt_http_server_example
    server_example.cc
)
target_link_libraries(bbt_http_server_example bbt_http)
