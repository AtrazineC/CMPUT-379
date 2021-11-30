cmake_minimum_required(VERSION 3.16)
project(Assignment3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(src/Common)

add_executable(
        Server
        src/Server/server.cpp
        src/Server/server_logger.cpp src/Server/server_logger.h
        src/Common/util.cpp src/Common/util.h
        src/Common/common.h
        src/Common/tands.cpp src/Common/tands.h
        src/Server/transaction_handler.cpp src/Server/transaction_handler.h
        src/Server/server_connection_handler.cpp src/Server/server_connection_handler.h
        src/Server/fd_array_handler.cpp src/Server/fd_array_handler.h)

add_executable(
        Client
        src/Client/client.cpp
        src/Client/command.cpp src/Client/command.h
        src/Client/client_logger.cpp src/Client/client_logger.h
        src/Common/util.cpp src/Common/util.h
        src/Common/common.h
        src/Common/tands.cpp src/Common/tands.h
        src/Client/client_connection_handler.cpp src/Client/client_connection_handler.h)
