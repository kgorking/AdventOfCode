import aoc;
#include "../../aoc/catch2_macro_guards.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

constexpr auto get_sample_input() {
	return std::to_array<std::string_view>({
		#include "sample_input.txt"
	});
}

constexpr auto get_input() {
	return std::to_array<std::string_view>({
#		include "input.txt"
	});
}

#include "impl.cpp"

TEST_CASE("validate") {
	auto const input = get_input();
	auto const sample_input = get_sample_input();

	SECTION("sample input for part 1 & 2") {
		auto const [p1, p2] = solve(sample_input);
		REQUIRE(p1 == 0);
		REQUIRE(p2 == 0);
	}

	SECTION("actual input") {
		auto const [p1, p2] = solve(input);
		std::cout << "Part 1: " << p1 << '\n';
		std::cout << "Part 2: " << p2 << '\n';
		REQUIRE(p1 == 0);
		REQUIRE(p2 == 0);
	}
}

TEST_CASE("benchmark") {
	auto input = get_input();

	BENCHMARK("Part 1+2") {
		auto const [p1, p2] = solve(std::move(input));
		return p1 | p2;
	};
}
