add_executable(todo_server 
    core.cc
    server.cc
)
target_link_libraries(todo_server cppboot_base cppboot_adv cppboot_net)

add_executable(todo
    core.cc
    todo.cc
)

target_link_libraries(todo cppboot_base cppboot_adv)


add_executable(todo_test
    core.cc
    core_test.cc
)

target_link_libraries(todo_test cppboot_base gmock gmock_main)
add_test(NAME example_todo COMMAND todo_test)