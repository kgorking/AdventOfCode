﻿cmake_minimum_required (VERSION 3.28)

project (AdventOfCode CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# Catch2
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.6.0 # or a later release
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

# AoC module lib
add_library(AdventOfCode STATIC)
target_compile_options(AdventOfCode PRIVATE /c /EHsc /nologo)

target_sources(AdventOfCode
  PUBLIC
    FILE_SET CXX_MODULES FILES
		"aoc/std.ixx"
		"aoc/aoc.ixx"
		"aoc/aoc-bits.ixx"
		"aoc/aoc-pos2d.ixx"
		"aoc/aoc-matrix.ixx"
		"aoc/aoc-numeric.ixx"
		"aoc/aoc-graph.ixx"
		"aoc/aoc-range.ixx"
		"aoc/aoc-math.ixx"
		"aoc/aoc-tlssplit.ixx"
		"aoc/aoc-async.ixx"
		"aoc/aoc-array.ixx"  "aoc/aoc-string.ixx")


# Include sub-projects.
add_subdirectory ("common_unittests")
#add_subdirectory ("2015")
#add_subdirectory ("2020")
#add_subdirectory ("2021")
#add_subdirectory ("2022")
add_subdirectory ("2023")
