# https://www.internalpointers.com/post/modern-cmake-beginner-introduction
cmake_minimum_required(VERSION 3.11.3)

project(Airport-traffic
    VERSION 1.0
    DESCRIPTION "Aiport traffic simulator"
    LANGUAGES CXX)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

# Find the OpenCV Libraries
find_package(OpenCV 4.1 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

file(GLOB PROJECT_SRCS CONFIGURE_DEPENDS src/*.cpp) 

add_executable(airport_traffic ${PROJECT_SRCS})
target_link_libraries(airport_traffic ${OpenCV_LIBRARIES})