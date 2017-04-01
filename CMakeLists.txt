cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(socketTest CXX)

add_executable (server server.cpp)
target_link_libraries (server)
target_compile_features(server PUBLIC cxx_range_for)

add_executable (client client.cpp)
target_link_libraries (client)
target_compile_features (client PRIVATE cxx_range_for)