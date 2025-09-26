cmake_minimum_required (VERSION 3.12)

project (Caches LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (COMMON_COMPILE_OPTIONS
    $<$<CXX_COMPILER_ID:GNU,Clang>: -Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX>)

add_executable (arc
    src/driver_arc.cpp)
target_include_directories (arc PRIVATE include)
target_compile_options (arc PRIVATE ${COMMON_COMPILE_OPTIONS})

add_executable (ideal
    src/driver_ideal.cpp)
target_include_directories (ideal PRIVATE include)
target_compile_options (ideal PRIVATE ${COMMON_COMPILE_OPTIONS})
