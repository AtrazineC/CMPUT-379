cmake_minimum_required(VERSION 3.16)
project(Assignment2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

add_executable(Assignment2 src/main.cpp src/tands.cpp src/tands.h src/task_manager.cpp src/task_manager.h src/shared_data.cpp src/shared_data.h src/consumer.cpp src/consumer.h src/command.cpp src/command.h)
