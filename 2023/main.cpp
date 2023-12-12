import aoc;
#include "../aoc/catch2_macro_guards.h"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include AOC_IMPL_H

TEST_CASE("Sample input") {
	auto const [sample_1, sample_2] = solve(sample_input);
	std::cout << std::format("\nSample Part 1: {}\nSample Part 2: {}\n", sample_1, sample_2);

	auto const [expected_1, expected_2] = expected_sample;
	CHECK  (expected_1 == sample_1);
	REQUIRE(expected_2 == sample_2);
}

TEST_CASE("Actual input") {
	auto const [part_1, part_2] = solve(actual_input);
	std::cout << std::format("\nPart 1: {}\nPart 2: {}\n", part_1, part_2);

	auto const [expected_part_1, expected_part_2] = expected_input;
	CHECK  (expected_part_1 == part_1);
	REQUIRE(expected_part_2 == part_2);
}

TEST_CASE("Benchmark") {
	BENCHMARK("Part 1") {
		return part1(actual_input);
	};
	BENCHMARK("Part 2") {
		return part2(actual_input);
	};
	BENCHMARK("All") {
		return solve(actual_input);
	};
}
