add_library(bbt_util STATIC
    fs.cc
    logging.cc
    status.cc
    str_util.cc
    string_view.cc
    sys.cc
)

add_executable(bbt_util_test
    fs_test.cc
    logging_test.cc
    status_test.cc
    str_util_test.cc
    string_view_test.cc
    fmt_test.cc
    json_test.cc
    )
target_link_libraries(bbt_util_test bbt_util gmock gmock_main)
add_test(NAME bbt_util COMMAND bbt_util_test)