import aoc;
import std;
#define DAY "day05"

// Holds the expected values for sample + input. Used in unit tests.
static constexpr auto expected_sample = std::make_pair(3, 14);
static constexpr auto expected_input = std::make_pair(707, 361615643045059);

using range = kg::range<std::int64_t>;

auto make_interval_map(std::ranges::range auto const& rng) {
	std::map<std::int64_t, bool> intervals;

	// Entire 2^64 ids are spoiled to start with
	intervals[std::numeric_limits<std::int64_t>::min()] = 1;
	intervals[std::numeric_limits<std::int64_t>::max()] = 0;

	for (auto r : rng) {
		if (auto it = std::prev(intervals.lower_bound(r.first-1)); it->second == 1)
			intervals[r.first-1] = 0;

		if (auto it = std::prev(intervals.lower_bound(r.last)); it->second == 1)
			intervals.erase(it);

		intervals[r.last] = 1;
	}

	return intervals;
}

static auto part1(auto const& input) {
	auto [arr_ranges, ids] = input;
	std::ranges::sort(arr_ranges);

	auto const fresh_intervals = make_interval_map(arr_ranges);
	int count_fresh = 0;
	for (auto id : ids)
		count_fresh += fresh_intervals.lower_bound(id)->second;

	return count_fresh;
}

static auto part2(auto const& input) {
	auto [arr_ranges, ids] = input;
	std::ranges::sort(arr_ranges);

	auto it = arr_ranges.begin();
	range current = *it;
	std::int64_t total = 0;

	while (++it != arr_ranges.end()) {
		if (current.overlaps(*it))
			current = range::merge(current, *it);
		else {
			total += current.size();
			current = *it;
		}
	}
	total += current.size();

	return total;
}

// Tests and benchmarks
#include "../tests_and_benchmarks.h"
