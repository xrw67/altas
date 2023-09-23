add_library(bbt_net2
    buffer.cpp
)

add_executable(bbt_net2_test
    buffer_test.cpp
)
target_link_libraries(bbt_net2_test bbt_net2 gmock gmock_main)
add_test(NAME bbt_net2 COMMAND bbt_net2_test)

