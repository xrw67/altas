SET(CPPBOOT_ADV_SRCS
    args.cc
    env.cc
    module_manager.cc
    module_loader.cc
)

list(APPEND CPPBOOT_ADV_SRCS
    bus/client.cc
    bus/server.cc
    bus/msg.cc
    bus/msg_packer.cc
    bus/context.cc
    bus/invoker.cc
    bus/service.cc
    bus/router.cc
)

SET(CPPBOOT_ADV_TEST_SRCS
    args_test.cc
    env_test.cc
    module_test.cc
)

list(APPEND CPPBOOT_ADV_TEST_SRCS
    bus/client_test.cc
    bus/server_test.cc
    bus/msg_test.cc
    bus/msg_packer_test.cc
    bus/invoker_test.cc
    bus/service_test.cc
    bus/router_test.cc
)


add_library(cppboot_adv ${CPPBOOT_ADV_SRCS})
target_link_libraries(cppboot_adv cppboot_base cppboot_net)

add_executable(cppboot_adv_test ${CPPBOOT_ADV_TEST_SRCS})
target_link_libraries(cppboot_adv_test cppboot_adv gmock gmock_main)
add_test(NAME cppboot_adv COMMAND cppboot_adv_test)