import AOC_MODULE;
import std;
#include "../aoc/catch2_macro_guards.h"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

constexpr auto get_sample_input() {
	return
#include AOC_sample
	;
}

constexpr auto get_input() {
	return
#include AOC_input
	;
}
const auto sample_input = get_sample_input();
const auto actual_input = get_input();

TEST_CASE(AOC_SZ " - sample") {
	auto const part_1 = part1(sample_input);
	auto const part_2 = part2(sample_input);

	auto const [expected_part_1, expected_part_2] = expected_sample;
	CHECK(part_1 == expected_part_1);
	REQUIRE(part_2 == expected_part_2);

	std::cout << std::format(AOC_SZ "\n  Sample Part 1: {}\n  Sample Part 2: {}\n\n", part_1, part_2);
}

TEST_CASE(AOC_SZ " - actual") {
	auto const [expected_part_1, expected_part_2] = expected_input;

	auto const part_1 = part1(actual_input);
	REQUIRE(part_1 == expected_part_1);

	auto const part_2 = part2(actual_input);
	REQUIRE(part_2 == expected_part_2);

	std::cout << std::format(AOC_SZ "\n  Part 1: {}\n  Part 2: {}\n", part_1, part_2);
}

// Benchmarks are only run if tests pass in release mode
#ifndef _DEBUG
TEST_CASE(AOC_SZ) {
	BENCHMARK("Part 1") {
		return part1(actual_input);
	};
	BENCHMARK("Part 2") {
		return part2(actual_input);
	};
}
#endif
