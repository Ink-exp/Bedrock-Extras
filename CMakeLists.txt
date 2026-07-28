cmake_minimum_required(VERSION 3.18.1)
project("bedrock_extras" CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

add_library(
    bedrock_extras
    SHARED
    src/main.cpp
)

find_library(log-lib log)

target_link_libraries(
    bedrock_extras
    ${log-lib}
)
