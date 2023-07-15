add_executable(bbt_tests 
    test_hello.cc
    )
target_link_libraries(bbt_tests 
    gmock 
    gmock_main
    bbt_util 
    bbt_net 
    bbt_appkit
    )
add_test(NAME bbt_integration_test COMMAND bbt_tests)
