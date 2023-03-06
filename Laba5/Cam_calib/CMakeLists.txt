cmake_minimum_required(VERSION 3.16)
project(Laba_5_CLion)

set(CMAKE_CXX_STANDARD 14)

add_executable(Laba_5_CLion main.cpp aruco_samples_utility.h)

set(OpenCV_DIR "C:\\WIP\\Tex3penie\\OpenCV_2sem\\build\\install")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_aruco)

target_link_libraries(Laba_5_CLion ${OpenCV_LIBS})