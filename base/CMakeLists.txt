add_library(bbt_base SHARED 
    fs.cc
    logging.cc
    status.cc
    str_util.cc
    string_view.cc
    sys_util.cc
)

if(BBT_BUILD_TESTS)
    enable_testing()
    include(GoogleTest)


    add_executable(test_base 
        fs_test.cc    
        logging_test.cc
        status_test.cc
        str_util_test.cc
        string_view_test.cc
    )

    target_link_libraries(test_base bbt_base gmock gmock_main)
    gtest_discover_tests(test_base)

endif()