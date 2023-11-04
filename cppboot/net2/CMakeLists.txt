add_library(cppboot_net2
    buffer.cpp
    addr.cpp
)

add_executable(cppboot_net2_test
    buffer_test.cpp
    addr_test.cpp
)
target_link_libraries(cppboot_net2_test cppboot_net2 gmock gmock_main)
add_test(NAME cppboot_net2 COMMAND cppboot_net2_test)

