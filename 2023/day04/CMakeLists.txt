cmake_minimum_required (VERSION 3.24)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME DEST)

add_executable (${DEST})
target_compile_features(${DEST} PRIVATE cxx_std_20)
target_compile_options(${DEST} PRIVATE /EHsc /nologo /experimental:module /reference std=CMakeFiles\\AdventOfCode.dir\\std.ifc)
target_link_libraries(${DEST} AdventOfCode)

target_sources(${DEST}
  PRIVATE
    main.cpp
)
