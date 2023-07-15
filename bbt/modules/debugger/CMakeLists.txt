add_library(mod_debugger debugger.cc)
target_link_libraries(mod_debugger bbt_util bbt_appkit)

add_executable(bbt_mod_debugger_test
    debugger_test.cc
    )
target_link_libraries(bbt_mod_debugger_test mod_debugger gmock gmock_main)
add_test(NAME bbt_mod_debugger COMMAND bbt_mod_debugger_test)