add_library(bbt_appkit 
    args.cc
    env.cc
    module/manager.cc
    module/loader.cc
)

if(BBT_BUILD_TESTS)
    enable_testing()
    include(GoogleTest)


    add_executable(test_appkit   
        args_test.cc
        env_test.cc
        module/module_test.cc
    )
    
    target_link_libraries(test_appkit bbt_appkit bbt_base gmock gmock_main)
    gtest_discover_tests(test_appkit)

endif()