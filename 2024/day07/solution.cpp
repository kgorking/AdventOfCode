export module aoc.day07;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(3749ll, 11387ll);
export constexpr auto expected_input = std::make_pair(4122618559853ll, 227615740238334ll);

using input_t = std::span<const std::pair<std::int64_t, std::vector<int>>>;
using func = std::int64_t (*)(std::int64_t, std::int64_t);

func add = +[](std::int64_t a, std::int64_t b) { return a + b; };
func mul = +[](std::int64_t a, std::int64_t b) { return a * b; };
func cat = +[](std::int64_t a, std::int64_t b) {
	int count = 1;
	while (b / count > 0) {
		count *= 10;
	}
	return (a * count) + b;
};

bool calc(std::int64_t const value, std::int64_t const acc, std::span<const func> ops, auto i, auto const end) {
	if (i == end) {
		return value == acc;
	}

	if (acc > value) {
		return false;
	}

	for (func op : ops) {
		std::int64_t const v_op = op(acc, *i);
		if (calc(value, v_op, ops, i + 1, end))
			return true;
	}

	return false;
}

export auto part1(input_t input) {
	std::array<func, 2> ops { add, mul };

	std::int64_t sum = 0;
	for (auto const [value, calibrations] : input) {
		sum += value * calc(value, 0, ops, calibrations.begin(), calibrations.end());
	}
	return sum;
}

export auto part2(input_t input) {
	std::array<func, 3> ops { add, mul, cat };
	std::int64_t sum = 0;
	for (auto const [value, calibrations] : input) {
		sum += value * calc(value, 0, ops, calibrations.begin(), calibrations.end());
	}
	return sum;
}
