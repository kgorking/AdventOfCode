import aoc;
#include "../aoc/catch2_macro_guards.h"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

bool aoc_dev_mode = false;
#include AOC_IMPL_H

#include AOC_SAMPLE_H
TEST_CASE("Sample input") {
	auto const part_1 = part1(sample_input);
	auto const part_2 = part2(sample_input);
	std::cout << std::format("\nSample Part 1: {}\nSample Part 2: {}\n", part_1, part_2);

	auto const [expected_part_1, expected_part_2] = expected_sample;
	CHECK  (part_1 == expected_part_1);
	REQUIRE(part_2 == expected_part_2);
}

#include AOC_INPUT_H
TEST_CASE("Actual input") {
	if (aoc_dev_mode)
		return;
	auto const part_1 = part1(actual_input);
	auto const part_2 = part2(actual_input);
	std::cout << std::format("\nPart 1: {}\nPart 2: {}\n", part_1, part_2);

	auto const [expected_part_1, expected_part_2] = expected_input;
	REQUIRE(part_1 == expected_part_1);
	REQUIRE(part_2 == expected_part_2);

	// Benchmarks are only run if tests pass nad in relase mode
	#ifndef _DEBUG
	BENCHMARK("Part 1") {
		return part1(actual_input);
	};
	BENCHMARK("Part 2") {
		return part2(actual_input);
	};
	#endif
}
