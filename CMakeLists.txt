# MIT License
# Copyright (c) 2025 Chat System Project

cmake_minimum_required(VERSION 3.16)
project(ChatSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find threads library
find_package(Threads REQUIRED)

# Add subdirectories
add_subdirectory(protocol)
add_subdirectory(server)
add_subdirectory(client_gui)
