cmake_minimum_required(VERSION 3.23)
project(Server_Communication C)

set(CMAKE_C_STANDARD 17)

add_executable(client client.c)
add_executable(server server.c)
