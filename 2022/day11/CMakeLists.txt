cmake_minimum_required (VERSION 3.8)

file(TO_CMAKE_PATH $ENV{VCToolsInstallDir} VC_ToolsInstallDir)

add_executable (2022_day11 "${VC_ToolsInstallDir}/modules/std.ixx" "main.cpp")
target_compile_options(2022_day11 PUBLIC /EHsc /nologo /W4 /MTd /experimental:module)
