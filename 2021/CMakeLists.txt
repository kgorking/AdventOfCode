﻿# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.12)

project (AoC_2021 CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include sub-projects.
add_subdirectory ("day1")
add_subdirectory ("day2")
add_subdirectory ("day3")
add_subdirectory ("day4")
add_subdirectory ("day5")
add_subdirectory ("day6")
add_subdirectory ("day7")
add_subdirectory ("day8")
add_subdirectory ("day9")
add_subdirectory ("day10")
add_subdirectory ("day11")
add_subdirectory ("day12")
add_subdirectory ("day13")
add_subdirectory ("day14")
add_subdirectory ("day15")
add_subdirectory ("day16")
#add_subdirectory ("day17")
#add_subdirectory ("day18")
#add_subdirectory ("day19")
add_subdirectory ("day20")
add_subdirectory ("day21")
