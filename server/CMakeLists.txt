# MIT License
# Copyright (c) 2025 Chat System Project

add_executable(chat_server
    chat_server.h
    chat_server.cpp
    client_handler.h
    client_handler.cpp
    main.cpp
)

target_link_libraries(chat_server 
    PRIVATE 
    protocol
    Threads::Threads
)

target_include_directories(chat_server PRIVATE ${CMAKE_SOURCE_DIR}/protocol)
