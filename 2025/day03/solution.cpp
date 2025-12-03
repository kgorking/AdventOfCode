import aoc;
import std;
#define DAY "day03"

// Holds the expected values for sample + input. Used in unit tests.
static constexpr auto expected_sample = std::make_pair(  357, 3121910778619);
static constexpr auto expected_input  = std::make_pair(16854, 167526011932478);

// Solve it using a monotonic stack approach
static std::int64_t max_joltage_n(std::string_view battery_bank, int const s) {
	int n = battery_bank.size() - s;

	std::vector<char> mono;
	mono.reserve(battery_bank.size());

	for (char battery : battery_bank) {
		while (!mono.empty() && n > 0 && mono.back() < battery) {
			mono.pop_back();
			--n;
		}

		mono.push_back(battery);
	}

	while (n) {
		mono.pop_back();
		--n;
	}

	std::int64_t result = 0;
	while (n < mono.size()) {
		result *= 10;
		result += mono[n++];
		result -= '0';
	}

	return result;
}

static auto part1(auto const& input) {
	std::int64_t sum = 0;
	for (std::string_view battery_bank : input) {
		sum += max_joltage_n(battery_bank, 2);
	}
	return sum;
}

static auto part2(auto const& input) {
	std::int64_t sum = 0;
	for (std::string_view battery_bank : input) {
		sum += max_joltage_n(battery_bank, 12);
	}
	return sum;
}

// Tests and benchmarks
#include "../tests_and_benchmarks.h"
