cmake_minimum_required(VERSION 3.5)
project(ha1)

set(CMAKE_CXX_STANDARD 14)
add_definitions("-Wall -Werror")

set(SOURCE_FILES main.cpp graph.h graph_iterator.h filtered_graph.h shortest_path.h utils.h)
add_executable(ha1 ${SOURCE_FILES})
