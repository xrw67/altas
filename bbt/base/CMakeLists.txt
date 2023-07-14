add_library(bbt_base 
    fs.cc
    logging.cc
    status.cc
    str_util.cc
    string_view.cc
    sys.cc
)

add_executable(bbt_base_test
    fs_test.cc
    logging_test.cc
    status_test.cc
    str_util_test.cc
    string_view_test.cc
    )
target_link_libraries(bbt_base_test bbt_base gmock gmock_main)
add_test(NAME bbt_base COMMAND bbt_base_test)