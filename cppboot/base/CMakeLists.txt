add_library(cppboot_base STATIC
    fs.cc
    log.cc
    status.cc
    str_util.cc
    string_view.cc
    sys.cc
    timestamp.cc
)

add_executable(cppboot_base_test
    fs_test.cc
    log_test.cc
    status_test.cc
    str_util_test.cc
    string_view_test.cc
    fmt_test.cc
    json_test.cc
    sys_test.cc
    timestamp_test.cc
    )
target_link_libraries(cppboot_base_test cppboot_base gmock gmock_main)
add_test(NAME cppboot_base COMMAND cppboot_base_test)