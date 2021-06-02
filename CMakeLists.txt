cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

project(WaveWriter)

include_directories(include)

add_executable(${PROJECT_NAME} example/example.cpp)
