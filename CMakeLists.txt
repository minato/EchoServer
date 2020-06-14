cmake_minimum_required(VERSION 3.16)
project(EchoServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(EchoServer main.cpp EchoServer.cpp EchoServer.h)