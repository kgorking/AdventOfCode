cmake_minimum_required (VERSION 3.24)

# https://gitlab.kitware.com/cmake/cmake/-/commit/48502a378136e919454895235d722a386e1c4219
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "3c375311-a3c9-4396-a187-3227ef642046")

set(YEAR 2022)
set(DAY 0)

set(DEST ${YEAR}_day${DAY})
add_executable (${DEST})
target_compile_features(${DEST} PRIVATE cxx_std_20)
target_compile_options(${DEST} PRIVATE /experimental:module /reference "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${DEST}.dir/std.ifc")

target_sources(${DEST}
  PRIVATE
    main.cpp
  PRIVATE
    FILE_SET CXX_MODULES
      FILES
        std.ixx
)
