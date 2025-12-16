# MIT License
# Copyright (c) 2025 Chat System Project

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(chat_client
    socket_client.h
    socket_client.cpp
    main_window.h
    main_window.cpp
    main_window.ui
    main.cpp
)

target_link_libraries(chat_client 
    PRIVATE 
    protocol
    Qt5::Widgets
    Threads::Threads
)

target_include_directories(chat_client PRIVATE ${CMAKE_SOURCE_DIR}/protocol)
