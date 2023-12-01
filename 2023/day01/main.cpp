import aoc;
using namespace std::string_view_literals;

constexpr auto input = std::to_array<std::string_view>({
#include "input.txt"
});

// The values to search for
static constexpr auto values = std::to_array<std::string_view>(
	{"1", "2", "3", "4", "5", "6", "7", "8", "9", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"});

// Convert the values to searchers
static constexpr auto searchers  = values | std::views::transform([](auto phrase) { return std::boyer_moore_searcher(phrase. begin(), phrase. end()); });
static constexpr auto rsearchers = values | std::views::transform([](auto phrase) { return std::boyer_moore_searcher(phrase.rbegin(), phrase.rend()); });

int digits_to_int(char l, char r) {
	return (l - '0')*10 + (r - '0');
}

// Returns the combined character value of digits found first and last in `calibration`
int find_value_p1(std::string_view calibration) {
	auto const l = std::ranges::find_if(calibration, [](char c) { return std::isdigit(c); });
	auto const r = std::ranges::find_last_if(calibration, [](char c) { return std::isdigit(c); });

	return digits_to_int(*l, r[0]);
}

// Returns the combined character value of `::values` found first and last in `calibration`
int find_value_p2(std::string_view calibration) {
	std::size_t  index = values.size();
	std::size_t rindex = values.size();

	auto  min = calibration. end();
	auto rmin = calibration.rend();
	for (std::size_t i = 0; i < values.size(); i++) {
		auto const it  = std::search(calibration. begin(), calibration. end(),  searchers[i]);
		auto const rit = std::search(calibration.rbegin(), calibration.rend(), rsearchers[i]);

		if (calibration.end() != it && it < min) {
			index = i;
			min = it;
		}

		if (calibration.rend() != rit && rit < rmin) {
			rindex = i;
			rmin = rit;
		}
	}

	return digits_to_int(values[index % 9][0], values[rindex % 9][0]);
}

int main() {
	// Part 1
	int const sum1 = std::transform_reduce(input.begin(), input.end(), 0, std::plus{}, find_value_p1);
	std::cout << "Part 1: " << sum1 << '\n';

	// Part 2
	int const sum2 = std::transform_reduce(input.begin(), input.end(), 0, std::plus{}, find_value_p2);
	std::cout << "Part 2: " << sum2 << '\n';
}
