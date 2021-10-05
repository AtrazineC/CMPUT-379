cmake_minimum_required(VERSION 3.16)
project (Assignment1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Assignment1 main.cpp util.cpp util.h command.cpp command.h process.cpp process.h process_handler.cpp process_handler.h signal_handler.cpp signal_handler.h)
