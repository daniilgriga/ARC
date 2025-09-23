cmake_minimum_required (VERSION 3.12)

project (ARC LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (${PROJECT_NAME}
                src/driver_arc.cpp)

target_include_directories (ARC PRIVATE include)

target_compile_options (${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>: -Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX>)
