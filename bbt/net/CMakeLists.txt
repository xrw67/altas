add_library(bbt_net 
    ipaddr.cc
    http/http_context.cc
)
target_link_libraries(bbt_net bbt_util)

add_executable(bbt_net_test
    ipaddr_test.cc
    http/http_request_test.cc
    )
target_link_libraries(bbt_net_test bbt_net gmock gmock_main)
add_test(NAME bbt_net COMMAND bbt_net_test)