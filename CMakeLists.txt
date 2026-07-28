cmake_minimum_required(VERSION 3.18.1)
project("culling_plugin" CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source directory inclusion
include_directories(src)

# Shared library binary target
add_library(
    culling_plugin
    SHARED
    src/main.cpp
)

# Link against Android NDK logging library
find_library(log-lib log)

target_link_libraries(
    culling_plugin
    ${log-lib}
)
