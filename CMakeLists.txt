cmake_minimum_required(VERSION 3.18.1)
project("bedrock_extras" CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    dobby
    GIT_REPOSITORY https://github.com/jmpews/Dobby.git
    GIT_TAG master
)
FetchContent_MakeAvailable(dobby)

include_directories(
    src 
    ${dobby_SOURCE_DIR}/include
)

add_library(
    bedrock_extras
    SHARED
    src/main.cpp
)

find_library(log-lib log)
find_library(GLESv2-lib GLESv2)
find_library(EGL-lib EGL)

target_link_libraries(
    bedrock_extras
    ${log-lib}
    ${GLESv2-lib}
    ${EGL-lib}
    dobby
)
