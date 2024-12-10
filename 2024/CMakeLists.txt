cmake_minimum_required (VERSION 3.28)

project (aoc_2024 CXX)

function(add_day)
	list(SUBLIST ARGV 1 -1 subdirs)
	foreach(dir IN LISTS subdirs)
		add_executable(${ARGV0}_${dir} "impl.cpp")

		target_sources(AdventOfCode PUBLIC FILE_SET CXX_MODULES FILES
			"${ARGV0}/solution.cpp"
		)

		target_compile_definitions(${ARGV0}_${dir} PRIVATE
			AOC_input="../2024/${ARGV0}/input.txt"
			AOC_sample="../2024/${ARGV0}/sample_input.txt"
			AOC_MODULE=aoc.${ARGV0}
			AOC_SZ="2024/${ARGV0}/${dir}"
			AOC_MAIN_CPP="../2024/${ARGV0}/${dir}.cpp"
			AOC_NS=aoc2024_${ARGV0}_${dir}
		)

		target_link_libraries(${ARGV0}_${dir} PRIVATE AdventOfCode Catch2::Catch2WithMain)
		catch_discover_tests(${ARGV0}_${dir})

	endforeach()
endfunction()


add_day (day01 solution)
add_day (day02 solution)
add_day (day03 solution)
add_day (day04 solution)
add_day (day05 solution)
add_day (day06 solution)
add_day (day07 solution)
add_day (day08 solution)
add_day (day09 solution)
add_day (day10 solution)
add_day (day11 solution)
#add_day (day12 solution)
#add_day (day13 solution)
#add_day (day14 solution)
#add_day (day15 solution)
#add_day (day16 solution)
#add_day (day17 solution)
#add_day (day18 solution)
#add_day (day19 solution)
#add_day (day20 solution)
#add_day (day21 solution)
#add_day (day22 solution)
#add_day (day23 solution)
#add_day (day24 solution)
#add_day (day25 solution)
