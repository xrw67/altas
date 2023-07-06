if(BBT_BUILD_TESTS)
    enable_testing()
    include(GoogleTest)

    add_executable(test_bbt 
        test_hello.cc    
    )

    target_link_libraries(test_bbt bbt_base bbt_appkit gmock gmock_main)
    gtest_discover_tests(test_bbt)

endif()