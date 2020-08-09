cmake_minimum_required(VERSION 3.15)
project(RTInOneWeekend)
set (CMAKE_CXX_STANDARD 17)

if (MSVC)
    message(STATUS "Using MSVC")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

add_subdirectory(contrib)

include_directories(contrib/stb/include)
include_directories(contrib/indicators/single_include)
include_directories(contrib/taskflow)
include_directories(include)


file(GLOB HEADER_PATH ./include/*.h)
add_executable(RTInOneWeekend source/main.cpp ${HEADER_PATH})
