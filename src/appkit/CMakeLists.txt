add_library(bbt_appkit 
    args.cc
    env.cc
    module/manager.cc
    module/loader.cc
)
target_link_libraries(bbt_appkit bbt_base)

add_executable(bbt_appkit_test
    args_test.cc
    env_test.cc
    )
target_link_libraries(bbt_appkit_test bbt_appkit gmock gmock_main)
add_test(NAME bbt_appkit COMMAND bbt_appkit_test)