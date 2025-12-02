import std;
import aoc;
#define DAY "day02"

// Holds the expected values for sample + input. Used in unit tests.
static constexpr auto expected_sample = std::make_pair(1227775554, 0);
static constexpr auto expected_input = std::make_pair(13919717792, 0);

constexpr auto scales = std::to_array<std::int64_t>({ 0x0, 11, 101, 1001, 10001, 100001, 1000001, 10000001, 100000001, 1000000001 });

std::int64_t sum_valid_ids(std::string_view first, std::string_view last) {
	std::int64_t ifirst = 0, ilast = 0;
	std::int64_t start = 0, end = 0;
	std::int64_t sum = 0;

	std::from_chars(first.data(), first.data() + first.size(), ifirst);
	std::from_chars(last.data(), last.data() + last.size(), ilast);
	std::from_chars(first.data(), (1ull, first.data() + std::max(1ull, first.size() / 2)), start);
	std::from_chars(last.data(), last.data() + std::max(1ull, last.size() / 2) + (last.size() != first.size() || last.size() & 1), end);

	auto str = std::format("{} - {}:", ifirst, ilast);
	std::print("{} | ", str);

	if (start > 1)
		start -= 1;

	int counter = 0;
	for (int i = start; i <= end; ++i) {
		int const num_digits = std::floor(std::log10(i) + 1);
		auto scaled = i * scales[num_digits];

		if (scaled > ilast)
			break;

		if (scaled >= ifirst) {
			sum += scaled;
			std::print("{} | ", scaled);

			if (++counter >= 5) {
				counter = 0;
				std::print("\n{} | ", std::string(str.size(), ' '));
			}
		}
	}
	std::println();
	return sum;
}

static auto part1(auto const& input) {
	std::int64_t sum = 0;
	for (auto const parts : input | std::views::chunk(2)) {
		sum += sum_valid_ids(parts[0], parts[1]);
	}
	return sum;
}

static auto part2(auto const& input) {
	return 0;
}


// Tests and benchmarks
#include "../tests_and_benchmarks.h"
