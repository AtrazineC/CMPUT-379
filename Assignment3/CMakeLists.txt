cmake_minimum_required(VERSION 3.16)
project(Assignment3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Server src/Server/server.cpp)
add_executable(Client src/Client/client.cpp)
