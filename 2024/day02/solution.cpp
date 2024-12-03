export module aoc.day02;
import aoc;
import std;

export constexpr auto expected_sample = std::make_pair(2, 4);
export constexpr auto expected_input = std::make_pair(407, 459);

auto safe_report = [](auto const& report) {
	auto diffs = report | std::views::pairwise_transform(std::minus{});

	// Check increasing/decreasing values
	auto signs = diffs | std::views::transform(std::signbit<int>);
	if (auto const [min, max] = std::ranges::minmax(signs); min != max)
		return false;

	// Check deltas between adjacent rows
	auto deltas = diffs | std::views::transform(kg::abs); // can't use std::abs because that would make sense
	if (auto const [min, max] = std::ranges::minmax(deltas); min < 1 || max > 3)
		return false;

	return true;
};

export int part1(auto&& input) {
	return std::transform_reduce(input.begin(), input.end(), 0, std::plus{}, safe_report);
}

export auto part2(auto&& input) {
	auto masked_safe_report = [](auto const& report) {
		if (safe_report(report))
			return true;

		// Delete a level and check if it's safe.
		auto masked_report = [&](int index) {
			auto copy = report;
			copy.erase(copy.begin() + index);
			return safe_report(copy);
		};

		return std::ranges::any_of(std::views::iota(0ull, report.size()), masked_report);
	};

	return std::transform_reduce(input.begin(), input.end(), 0, std::plus{}, masked_safe_report);
}
