import aoc;
import std;
#define DAY "day01"

// Holds the expected values for sample + input. Used in unit tests.
static constexpr auto expected_sample = std::make_pair(3, 6);
static constexpr auto expected_input = std::make_pair(980, 5961);

static auto part1(auto const& input) {
	int start = 50;
	int code = 0;

	for (auto turn : input) {
		start = (start + turn) % 100;
		code += start == 0;
	}

	return code;
}

static auto part2(auto const& input) {
	int position = 50;
	int code = 0;

	for (auto turn : input) {
		// Find the new position
		int newpos = position + turn;

		// Count the number of full dial rotations
		int clicks = std::abs(newpos) / 100;

		// Check if zero was passed
		clicks += position > 0 && newpos <= 0;

		// Add it to the code
		code += clicks;

		// Update the position.
		// Account for modulo of negative numbers
		position = (newpos + (100i64 << 32)) % 100;
	}

	return code;
}

// Tests and benchmarks
#include "../tests_and_benchmarks.h"
