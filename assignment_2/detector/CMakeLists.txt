cmake_minimum_required(VERSION 3.10)
project(BulletDetector)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(detector detector.cpp detector.hpp)

target_link_libraries(detector ${OpenCV_LIBS})