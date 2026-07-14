cmake_minimum_required(VERSION 3.15)

project(elfParser)

option(BUILD_SHARED_LIBS "build shared libraries" ON)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} elfStruct)