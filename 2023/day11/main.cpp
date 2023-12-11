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

TEST_CASE("validate input for part 1 & 2") {
	auto const input = get_input();
	auto const sample_input = get_sample_input();

	auto const sample_1 = solve(sample_input, 2);
	auto const sample_2 = solve(sample_input, 10);
	auto const sample_3 = solve(sample_input, 100);
	auto const [expected_1, expected_2, expected_3] = expected_sample();
	CHECK(expected_1 == sample_1);
	CHECK(expected_2 == sample_2);
	REQUIRE(expected_3 == sample_3);

	SECTION("actual input") {
		auto const part_1 = solve(std::move(input), 1);
		auto part_2 = part_1;
		#ifdef _DEBUG
			part_2 = solve(std::move(input), 1'000'000);
		#else
			BENCHMARK("Part 2") {
				part_2 = solve(std::move(input), 1'000'000);
				return result;
			};
		#endif
		std::cout << "Part 1: " << part_1 << '\n';
		std::cout << "Part 2: " << part_2 << '\n';

		auto const [expected_part_1, expected_part_2] = expected_input();
		REQUIRE(expected_part_1 == part_1);
		REQUIRE(expected_part_2 == part_2);
	}
}
