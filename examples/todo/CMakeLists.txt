add_executable(todo_server 
    core.cc
    server.cc
)
target_link_libraries(todo_server bbt_base bbt_appkit bbt_http bbt_html)

add_executable(todo
    core.cc
    todo.cc
)

target_link_libraries(todo bbt_base bbt_appkit)



add_executable(todo_test
    core.cc
    core_test.cc
)

target_link_libraries(todo_test bbt_base gmock gmock_main)
add_test(NAME example_todo COMMAND todo_test)