cmake_minimum_required (VERSION 3.24)

add_executable (unittest "matrix.cpp")
target_link_libraries(unittest PRIVATE AdventOfCode)
