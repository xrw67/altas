#cppboot_html
add_library(cppboot_html
    document.cc
    elements/element.cc
    elements/table.cc
)
target_link_libraries(cppboot_html cppboot_base)


# cppboot_http_test
add_executable(cppboot_html_test
    html_test.cc
)
target_link_libraries(cppboot_html_test cppboot_html gmock gmock_main)
add_test(NAME cppboot_html COMMAND cppboot_html_test)

