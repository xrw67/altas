add_library(cppboot_adv 
    args.cc
    env.cc
    module/manager.cc
    module/loader.cc
)
target_link_libraries(cppboot_adv cppboot_base)

add_executable(cppboot_adv_test
    args_test.cc
    env_test.cc
    )
target_link_libraries(cppboot_adv_test cppboot_adv gmock gmock_main)
add_test(NAME cppboot_adv COMMAND cppboot_adv_test)