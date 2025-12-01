cmake_minimum_required (VERSION 4.1.0 FATAL_ERROR)

if (CMAKE_VERSION VERSION_GREATER "4.1.0")
	set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
else()
	message(FATAL_ERROR "Unknown uuid for CMake version: '${CMAKE_VERSION}', check the cmake github for new uuid")
endif()


set(CMAKE_CXX_MODULE_STD ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (AdventOfCode LANGUAGES CXX)


# Catch2
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

# std module
#add_library(std_module STATIC)
#target_sources(std_module PUBLIC FILE_SET CXX_MODULES FILES "aoc/std.ixx")

# AoC module lib
add_executable(Aoc_2025 "2025/dummy.cpp")

target_sources(Aoc_2025
  PUBLIC
    FILE_SET CXX_MODULES FILES
		"aoc/aoc.ixx"
		"aoc/aoc-bits.ixx"
		"aoc/aoc-direction.ixx"
		"aoc/aoc-pos2d.ixx"
		"aoc/aoc-matrix.ixx"
		"aoc/aoc-numeric.ixx"
		"aoc/aoc-graph.ixx"
		"aoc/aoc-range.ixx"
		"aoc/aoc-math.ixx"
		"aoc/aoc-tlssplit.ixx"
		"aoc/aoc-async.ixx"
		"aoc/aoc-array.ixx"
		"aoc/aoc-string.ixx"
		"aoc/aoc-ranges.ixx"
		"aoc/aoc-views.ixx"
		"aoc/aoc-cursor.ixx"
		"aoc/aoc-visited.ixx"
		"aoc/aoc-grid.ixx")


# Include sub-projects.
#add_subdirectory ("common_unittests")
#add_subdirectory ("2015")
#add_subdirectory ("2020")
#add_subdirectory ("2021")
#add_subdirectory ("2022")
#add_subdirectory ("2023")
#add_subdirectory ("2024")

# 2025
target_sources(Aoc_2025 PUBLIC
    "2025/day01/solution.cpp"
    "2025/day02/solution.cpp"
)


target_link_libraries(Aoc_2025 PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(Aoc_2025)
