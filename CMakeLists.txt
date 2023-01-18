cmake_minimum_required (VERSION 3.24)

project (AdventOfCode CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate MSVC stl as module. Enables 'import std;'
# https://gitlab.kitware.com/cmake/cmake/-/commit/48502a378136e919454895235d722a386e1c4219
# https://gitlab.kitware.com/cmake/cmake/-/blob/399d62db8b5edfa7f0ee9ea4c913b33dedba107b/Source/cmExperimental.cxx
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "3c375311-a3c9-4396-a187-3227ef642046")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)

add_library(AdventOfCode OBJECT)
target_compile_options(AdventOfCode PRIVATE /EHsc /nologo)

target_sources(AdventOfCode
  PUBLIC
    FILE_SET CXX_MODULES FILES
		"common/common.ixx"
 )


# Include sub-projects.
add_subdirectory ("common_unittests")
#add_subdirectory ("2015")
#add_subdirectory ("2020")
#add_subdirectory ("2021")
add_subdirectory ("2022")
