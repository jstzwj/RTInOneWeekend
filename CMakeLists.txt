cmake_minimum_required(VERSION 3.15)
project(RTInOneWeekend)
set (CMAKE_CXX_STANDARD 17)

add_subdirectory(contrib)

include_directories(contrib)
include_directories(include)


file(GLOB HEADER_PATH ./include/*.h)
add_executable(RTInOneWeekend source/main.cpp ${HEADER_PATH})
