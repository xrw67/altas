

add_library(mod_debugger debugger.cc)
target_link_libraries(mod_debugger bbt_base bbt_appkit)

if(BBT_BUILD_TESTS)
    enable_testing()
    include(GoogleTest)

    add_executable(test_mod_debugger debugger_test.cc)
    target_link_libraries(test_mod_debugger bbt_base bbt_appkit mod_debugger gmock gmock_main)
    gtest_discover_tests(test_mod_debugger)

endif()