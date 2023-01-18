cmake_minimum_required (VERSION 3.24)

set(DEST 2022_day8)

add_executable (${DEST})
target_compile_features(${DEST} PRIVATE cxx_std_20)
target_compile_options(${DEST} PRIVATE /EHsc /nologo /experimental:module /reference common=CMakeFiles\\AdventOfCode.dir\\common.ifc)
target_link_libraries(${DEST} AdventOfCode)

target_sources(${DEST}
  PUBLIC main.cpp
)
