import aoc;

constexpr char operational = '.';
constexpr char damaged = '#';
constexpr char unknown = '?';

struct input_t {
	std::string_view record;
	std::vector<char> groups;
};

constexpr auto part1(auto const& input) {
	// prune
	// backtrack

	return 0;
}

constexpr auto part2(auto const& input) {
	return 0;
}

auto solve(auto const& input) {
	return std::make_pair(part1(input), part2(input));
}

constexpr auto expected_sample = std::make_pair(0, 0);
constexpr auto expected_input = std::make_pair(0, 0);
const auto sample_input = std::to_array<input_t>({
	#include "sample_input.txt"
});
const auto actual_input = std::to_array<input_t>({
	#include "input.txt"
});
