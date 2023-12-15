import aoc;
#include "../aoc/catch2_macro_guards.h"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include AOC_IMPL_H
#include AOC_SAMPLE_H
#include AOC_INPUT_H

bool aoc_dev_mode = false;

TEST_CASE("Sample input") {
	//auto const [sample_1, sample_2] = solve(sample_input);
	auto const sample_1 = part1(sample_input);
	auto const sample_2 = part2(sample_input);
	std::cout << std::format("\nSample Part 1: {}\nSample Part 2: {}\n", sample_1, sample_2);

	auto const [expected_1, expected_2] = expected_sample;
	REQUIRE(expected_1 == sample_1);
	REQUIRE(expected_2 == sample_2);
}

TEST_CASE("Actual input") {
	if (aoc_dev_mode)
		return;
	auto const part_1 = part1(actual_input);
	auto const part_2 = part2(actual_input);
	std::cout << std::format("\nPart 1: {}\nPart 2: {}\n", part_1, part_2);

	auto const [expected_part_1, expected_part_2] = expected_input;
	REQUIRE(expected_part_1 == part_1);
	REQUIRE(expected_part_2 == part_2);

	// Benchmarks are only run if tests pass
	BENCHMARK("Part 1") {
		return part1(actual_input);
	};
	BENCHMARK("Part 2") {
		return part2(actual_input);
	};
	BENCHMARK("All") {
		return part1(actual_input);
		return part2(actual_input);
	};
}
