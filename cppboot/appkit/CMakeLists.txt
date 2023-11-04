add_library(cppboot_appkit 
    args.cc
    env.cc
    module/manager.cc
    module/loader.cc
)
target_link_libraries(cppboot_appkit cppboot_base)

add_executable(cppboot_appkit_test
    args_test.cc
    env_test.cc
    )
target_link_libraries(cppboot_appkit_test cppboot_appkit gmock gmock_main)
add_test(NAME cppboot_appkit COMMAND cppboot_appkit_test)