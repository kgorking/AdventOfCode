import aoc;
import std;
#define DAY "day03"

// Holds the expected values for sample + input. Used in unit tests.
static constexpr auto expected_sample = std::make_pair(  357, 0);
static constexpr auto expected_input  = std::make_pair(16854, 0);

int max_joltage_pair(std::string_view battery_bank) {
	std::string_view::iterator first, second;
	first = std::ranges::max_element(battery_bank);

	if (first == std::prev(battery_bank.end())) {
		second = std::ranges::max_element(battery_bank.begin(), first);
		std::swap(first, second);
	} else {
		second = std::ranges::max_element(std::ranges::subrange(std::next(first), battery_bank.end()));
	}

	return ((*first - '0') * 10) + *second - '0';
}

int max_joltage_12(std::string_view battery_bank) {
	// Find the collection of the twelve largest joltages in the battery bank

	return 0;
}

static auto part1(auto const& input) {
	std::int64_t sum = 0;
	for (std::string_view battery_bank : input) {
		int const joltage = max_joltage_pair(battery_bank);
		std::println("{} ", joltage);
		sum += joltage;
	}
	return sum;
}

static auto part2(auto const& input) {
	std::int64_t sum = 0;
	for (std::string_view battery_bank : input) {
		int const joltage = max_joltage_12(battery_bank);
		std::println("{} ", joltage);
		sum += joltage;
	}
	return sum;
}

// Tests and benchmarks
#include "../tests_and_benchmarks.h"
