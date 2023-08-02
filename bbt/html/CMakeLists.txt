#bbt_html
add_library(bbt_html
    document.cc
    elements/element.cc
    elements/table.cc
)
target_link_libraries(bbt_html bbt_base)


# bbt_http_test
add_executable(bbt_html_test
    html_test.cc
)
target_link_libraries(bbt_html_test bbt_html gmock gmock_main)
add_test(NAME bbt_html COMMAND bbt_html_test)

