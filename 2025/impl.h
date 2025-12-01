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

TEST_CASE(__FILE__ " - sample") {
	auto const [expected_part_1, expected_part_2] = expected_sample;

	auto const part_1 = part1(sample_input);
	CHECK(part_1 == expected_part_1);

	auto const part_2 = part2(sample_input);
	REQUIRE(part_2 == expected_part_2);

	std::print("{}\n  Sample Part 1: {}\n  Sample Part 2: {}\n\n", __FILE__, part_1, part_2);
}

TEST_CASE(__FILE__ " - actual") {
	auto const [expected_part_1, expected_part_2] = expected_input;

	auto const part_1 = part1(actual_input);
	REQUIRE(part_1 == expected_part_1);

	auto const part_2 = part2(actual_input);
	REQUIRE(part_2 == expected_part_2);

	std::print("{}\n  Part 1: {}\n  Part 2: {}\n", __FILE__, part_1, part_2);
}

// Benchmarks are only run if tests pass in release mode
#ifndef _DEBUG
TEST_CASE(__FILE__) {
	BENCHMARK("Part 1") {
		return part1(actual_input);
	};
	BENCHMARK("Part 2") {
		return part2(actual_input);
	};
}
#endif
