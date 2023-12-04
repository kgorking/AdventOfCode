cmake_minimum_required (VERSION 3.24)

project (AdventOfCode CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cxx_modules_rules.cmake)

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
		"aoc/aoc-graph.ixx"
		"aoc/aoc-range.ixx"
		"aoc/aoc-math.ixx"
		"aoc/aoc-tlssplit.ixx"
		"aoc/aoc-async.ixx"
		"aoc/aoc-array.ixx")


# Include sub-projects.
add_subdirectory ("common_unittests")
#add_subdirectory ("2015")
#add_subdirectory ("2020")
#add_subdirectory ("2021")
#add_subdirectory ("2022")
add_subdirectory ("2023")
