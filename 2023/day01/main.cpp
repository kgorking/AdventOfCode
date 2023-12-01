import aoc;
using namespace std::string_view_literals;

constexpr auto input = std::to_array<std::string_view>({
#include "input.txt"
});

static constexpr auto values = std::to_array<std::string_view>(
	{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"});

// Returns the character value of `::values` found first/last in `calibration`
template <bool reverse>
char find_first_value(std::ranges::range auto calibration) {
	// Convert the words to searchers
	static constexpr auto searchers = values | std::views::transform([](auto phrase) {
										  if constexpr (reverse)
											  return std::boyer_moore_searcher(phrase.rbegin(), phrase.rend());
										  else
											  return std::boyer_moore_searcher(phrase.begin(), phrase.end());
									  });

	std::size_t index = values.size();

	auto const search = [&](auto begin, auto end) {
		auto min = end;
		for (int i = 0; i < 20; i++) {
			auto const it = std::search(begin, end, searchers[i]);
			if (end != it && it < min) {
				index = i;
				min = it;
			}
		}
	};

	if constexpr (reverse) {
		search(calibration.rbegin(), calibration.rend());
	} else {
		search(calibration.begin(), calibration.end());
	}

	// Return the result as a char
	return values[index % 10][0];
}

int digits_to_int(char l, char r) {
	return (l - '0')*10 + (r - '0');
}

int extract_calibration_value_p2(int other, std::string_view calibration) {
	auto const l = find_first_value<false>(calibration);
	auto const r = find_first_value<true>(calibration);
	
	return other + digits_to_int(l, r);
}

int extract_calibration_value_p1(int other, std::string_view calibration) {
	auto const l = std::ranges::find_if(calibration, [](char c){ return std::isdigit(c);});
	auto const r = std::ranges::find_last_if(calibration, [](char c){ return std::isdigit(c);});

	return other + digits_to_int(*l, r[0]);
}

int main() {
	// Part 1
	int const sum1 = std::accumulate(input.begin(), input.end(), 0, extract_calibration_value_p1);
	std::cout << "Part 1: " << sum1 << '\n';

	// Part 2
	int const sum2 = std::accumulate(input.begin(), input.end(), 0, extract_calibration_value_p2);
	std::cout << "Part 2: " << sum2 << '\n';
}
