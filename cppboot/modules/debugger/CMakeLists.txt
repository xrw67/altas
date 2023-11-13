add_library(mod_debugger debugger.cc)
target_link_libraries(mod_debugger cppboot_base cppboot_adv)

add_executable(cppboot_mod_debugger_test
    debugger_test.cc
    )
target_link_libraries(cppboot_mod_debugger_test mod_debugger gmock gmock_main)
add_test(NAME cppboot_mod_debugger COMMAND cppboot_mod_debugger_test)