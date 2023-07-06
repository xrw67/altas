add_library(bbt_appkit SHARED 
    module.cc
    env.cc
)

if(BBT_BUILD_TESTS)
    enable_testing()
    include(GoogleTest)


    add_executable(test_appkit 
        module_test.cc    
        env_test.cc
    )
    
    target_link_libraries(test_appkit bbt_appkit bbt_base gmock gmock_main)
    gtest_discover_tests(test_appkit)

endif()