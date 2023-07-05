

add_library(mod_hello SHARED main.cc)
target_link_libraries(mod_hello bbt_base bbt_appkit)