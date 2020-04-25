cmake_minimum_required(VERSION 3.8)
project(thickness_meter_test)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp Filter.cpp Filter.h FileReader.cpp FileReader.h)
add_executable(thickness_meter_test ${SOURCE_FILES})