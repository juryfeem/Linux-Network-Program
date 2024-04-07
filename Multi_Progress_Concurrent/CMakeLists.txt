cmake_minimum_required(VERSION 3.0)

project(SERVER)

add_executable(server server.c wrap.c)

# add_executable(client client.c wrap.c)