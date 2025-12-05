#include "../aoc/catch2_macro_guards.h"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

static constexpr auto get_sample_input() {
	return
		#include "sample_input.txt"
		;
}

static constexpr auto get_input() {
	return
		#include "input.txt"
		;
}

static const auto sample_input = get_sample_input();
static const auto actual_input = get_input();

TEST_CASE(DAY) {
	auto const [expected_sample_part_1, expected_sample_part_2] = expected_sample;
	auto const [expected_input_part_1, expected_input_part_2] = expected_input;

	auto const sample_part_1 = part1(sample_input);
	REQUIRE(sample_part_1 == expected_sample_part_1);

	auto const sample_part_2 = part2(sample_input);
	REQUIRE(sample_part_2 == expected_sample_part_2);


	auto const actual_part_1 = part1(actual_input);
	REQUIRE(actual_part_1 == expected_input_part_1);

	auto const actual_part_2 = part2(actual_input);
	REQUIRE(actual_part_2 == expected_input_part_2);

	std::println("{} - Sample {} / {}", DAY, sample_part_1, sample_part_2);
	std::println("{} - Actual {} / {}", DAY, actual_part_1, actual_part_2);
}

// Benchmarks are only run if tests pass in release mode
#ifndef _DEBUG
TEST_CASE(DAY " - benchmark") {
	BENCHMARK("Part 1") {
		return part1(actual_input);
	};
	BENCHMARK("Part 2") {
		return part2(actual_input);
	};
}
#endif
