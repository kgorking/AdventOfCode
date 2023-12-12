import aoc;

constexpr auto part1(auto const& input) {
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
constexpr auto sample_input = std::to_array<std::string_view>({
#include "sample_input.txt"
});
constexpr auto actual_input = std::to_array<std::string_view>({
#include "input.txt"
});
