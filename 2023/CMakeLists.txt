cmake_minimum_required (VERSION 3.24)

project (AoC_2023 CXX)

macro(add_day)
	add_executable (${ARGV0} "main.cpp")
	add_test(${ARGV0} ${ARGV0})

	target_compile_options(${ARGV0} PRIVATE /ZI /EHsc /nologo /experimental:module) #// /reference std=CMakeFiles\\AdventOfCode.dir\\std.ifc)
	target_link_options(${ARGV0} PRIVATE /INCREMENTAL /PROFILE /EDITANDCONTINUE)
	target_compile_definitions(${ARGV0} PRIVATE
		AOC_IMPL_H="./${ARGV0}/impl.cpp"
		AOC_SAMPLE_H="./${ARGV0}/sample_input.txt"
		AOC_INPUT_H="./${ARGV0}/input.txt"
	)
	target_link_libraries(${ARGV0} PRIVATE AdventOfCode Catch2::Catch2WithMain)
	#catch_discover_tests(${ARGV0})
endmacro()


add_subdirectory ("day01")
add_subdirectory ("day02")
add_subdirectory ("day03")
add_subdirectory ("day04")
add_subdirectory ("day05")
add_subdirectory ("day06")
add_day ("day07")
add_day ("day08")
add_day ("day09")
add_day ("day10")
add_day ("day11")
add_day ("day12")
add_day ("day13")
add_day ("day14")
add_day ("day15")
add_day ("day16")
add_day ("day17")
add_day ("day18")
add_day ("day19")
add_day ("day20")
add_day ("day21")
add_day ("day22")
add_day ("day23")
add_day ("day24")
add_day ("day25")
