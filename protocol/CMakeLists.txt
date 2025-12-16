# MIT License
# Copyright (c) 2025 Chat System Project

add_library(protocol STATIC
    message.h
    message.cpp
)

target_include_directories(protocol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
