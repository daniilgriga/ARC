cmake_minimum_required (VERSION 3.10)

project (ARC)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories (include)

file (GLOB SOURCES "src/*.cpp")

add_executable (${PROJECT_NAME} ${SOURCES})

target_compile_options (${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -std=c++20
    $<$<CONFIG:Debug>:-O0 -ggdb3>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)
