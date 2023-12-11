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
		auto const result = solve(sample_input);
		REQUIRE(result == expected_sample());
	}

	SECTION("actual input") {
		using result_t = decltype(solve(input));
		result_t result;
		BENCHMARK("Part 1+2") {
			result = solve(std::move(input));
			return result;
		};
		auto const [p1, p2] = result;
		std::cout << "Part 1: " << p1 << '\n';
		std::cout << "Part 2: " << p2 << '\n';
		REQUIRE(result == expected_input());
	}
}
