import std;
import aoc;
#define DAY "day02"

// Holds the expected values for sample + input. Used in unit tests.
static constexpr auto expected_sample = std::make_pair(1227775554, 4174379265);
static constexpr auto expected_input = std::make_pair(13919717792, 0);

constexpr auto scales = std::to_array({
	std::to_array<std::int64_t>({      0,           0,                0,                 0,             0,                0,                   0,                 0,          0 }),
	std::to_array<std::int64_t>({     11,         111,             1111,             11111,        111111,          1111111,            11111111,         111111111, 1111111111 }),
	std::to_array<std::int64_t>({    101,       10101,          1010101,         101010101,   10101010101,    1010101010101,     101010101010101, 10101010101010101,          0 }),
	std::to_array<std::int64_t>({   1001,     1001001,       1001001001,        1001001001, 1001001001001, 1001001001001001, 1001001001001001001,                 0,          0 }),
	std::to_array<std::int64_t>({  10001,   100010001,    1000100010001, 10001000100010001,             0,                0,                   0,                 0,          0 }),
	std::to_array<std::int64_t>({ 100001, 10000100001, 1000010000100001,                 0,             0,                0,                   0,                 0,          0 }),
	});

int count_digits(std::int64_t n) {
	return std::floor(std::log10(n) + 1);
}

std::int64_t sum_valid_ids(std::string_view first, std::string_view last) {
	std::int64_t start = 0, end = 0;
	std::int64_t sum = 0;

	std::int64_t const ifirst = std::atoi(first.data());
	std::int64_t const ilast = std::atoi(last.data());

	std::from_chars(first.data(), first.data() + std::max(1ull, first.size() / 2), start);
	std::from_chars(last.data(),  last.data()  + std::max(1ull, last.size() / 2) + (last.size() != first.size()), end);

	if (start > 1)
		start -= 1;

	int counter = 0;
	for (int i = start; i <= end; ++i) {
		int const num_digits = count_digits(i);
		auto scaled = i * scales[num_digits][0];

		if (scaled > ilast)
			break;

		if (scaled >= ifirst) {
			sum += scaled;
		}
	}

	return sum;
}

std::int64_t sum_valid_ids_p2(std::string_view first, std::string_view last) {
	std::int64_t start = 0, end = 0;
	std::int64_t sum = 0;

	std::int64_t const ifirst = std::atoi(first.data());
	std::int64_t const ilast = std::atoi(last.data());

	const auto half_size = std::max(1ull, (first.size()) / 2);

	//auto str = std::format("{} - {}:", ifirst, ilast);
	//std::print("{} | ", str);

	for (std::size_t s = 1; s <= half_size; ++s) {
		std::from_chars(first.data(), first.data() + s, start);
		std::from_chars(last.data(), last.data() + s + (last.size() != first.size()), end);

		if (start > 1)
			start -= 1;

		int counter = 0;
		for (int i = start; i <= end; ++i) {
			int const num_digits = count_digits(i);

			for (auto scale : scales[num_digits]) {
				auto scaled = i * scale;

				if (scaled > ilast)
					break;

				if (scaled >= ifirst) {
					sum += scaled;
					//std::print("{} | ", scaled);
					//
					//if (++counter >= 5) {
					//	counter = 0;
					//	std::print("\n{} | ", std::string(str.size(), ' '));
					//}
				}
			}
		}
	}
	//std::println();

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
	std::int64_t sum = 0;
	for (auto const parts : input | std::views::chunk(2)) {
		sum += sum_valid_ids_p2(parts[0], parts[1]);
	}
	return sum;
}


// Tests and benchmarks
#include "../tests_and_benchmarks.h"
