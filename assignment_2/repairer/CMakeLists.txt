cmake_minimum_required(VERSION 3.10)
project(Denoise)

find_package(OpenCV REQUIRED)

add_executable(denoise denoise.cpp)
target_link_libraries(denoise ${OpenCV_LIBS})