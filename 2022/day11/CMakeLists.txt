cmake_minimum_required (VERSION 3.8)
add_executable (2022_day11 "main.cpp")

target_compile_options(2022_day11 PUBLIC /EHsc /nologo /W4 /MTd /experimental:module /reference c:/stlmodules/std.ifc)
target_link_libraries(2022_day11 c:/stlmodules/std.obj)
