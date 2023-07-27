add_library(bbt_base STATIC
    fs.cc
    log.cc
    status.cc
    str_util.cc
    string_view.cc
    sys.cc
    timestamp.cc
)

add_executable(bbt_base_test
    fs_test.cc
    log_test.cc
    status_test.cc
    str_util_test.cc
    string_view_test.cc
    fmt_test.cc
    json_test.cc
    timestamp_test.cc
    )
target_link_libraries(bbt_base_test bbt_base gmock gmock_main)
add_test(NAME bbt_base COMMAND bbt_base_test)