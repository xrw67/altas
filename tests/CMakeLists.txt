add_executable(cppboot_tests 
    test_hello.cc
    )
target_link_libraries(cppboot_tests 
    gmock 
    gmock_main
    cppboot_base 
    cppboot_adv
    )
add_test(NAME cppboot_integration_test COMMAND cppboot_tests)
