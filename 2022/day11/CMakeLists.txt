cmake_minimum_required (VERSION 3.24)

# https://gitlab.kitware.com/cmake/cmake/-/commit/48502a378136e919454895235d722a386e1c4219
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "3c375311-a3c9-4396-a187-3227ef642046")

set(DEST 2022_day11)

add_executable (${DEST})
target_compile_features(${DEST} PRIVATE cxx_std_20)
target_compile_options(${DEST} PRIVATE /EHsc /nologo /W4 /MTd /experimental:module)

target_sources(${DEST}
  PRIVATE
    main.cpp
  PRIVATE
    FILE_SET CXX_MODULES
      FILES
        std.ixx
)
