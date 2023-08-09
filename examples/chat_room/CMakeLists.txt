add_executable(chat_room
    chat_room.cc
)

target_link_libraries(chat_room 
    bbt_base 
    bbt_net
)