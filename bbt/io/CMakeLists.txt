add_library(bbt_io
    context_kqueue.cpp
)
target_link_libraries(bbt_io bbt_base)

add_executable(bbt_io_test
    context_test.cpp
)
target_link_libraries(bbt_io_test bbt_io gmock gmock_main)
add_test(NAME bbt_io COMMAND bbt_io_test)