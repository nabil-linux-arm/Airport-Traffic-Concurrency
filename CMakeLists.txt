# https://www.internalpointers.com/post/modern-cmake-beginner-introduction
cmake_minimum_required(VERSION 3.11.3)

project(Airport-traffic
    VERSION 1.0
    DESCRIPTION "Aiport traffic simulator"
    LANGUAGES CXX)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

file(GLOB PROJECT_SRCS CONFIGURE_DEPENDS src/*.cpp) 

add_executable(airport_traffic ${PROJECT_SRCS})