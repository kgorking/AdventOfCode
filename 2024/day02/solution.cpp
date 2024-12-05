export module aoc.day02;
import aoc;
import std;

export constexpr auto expected_sample = std::make_pair(2, 4);
export constexpr auto expected_input = std::make_pair(407, 459);

constexpr auto safe_report = [](auto const& report) {
	auto diffs = report | std::views::pairwise_transform(std::minus{});

	// Check increasing/decreasing values
	if (auto const [min, max] = std::ranges::minmax(diffs | kg::views::signbit); min != max)
		return false;

	// Check deltas between adjacent rows
	if (auto const [min, max] = std::ranges::minmax(diffs | kg::views::abs); min < 1 || max > 3)
		return false;

	return true;
};

export int part1(auto&& input) {
	return kg::sum(input | std::views::transform(safe_report));
}

export auto part2(auto&& input) {
	auto dampened_safe_report = [](auto const& report) {
		// TODO return bad reports
		if (safe_report(report))
			return true;

		return std::ranges::any_of(std::views::iota(0ull, report.size()), [&](int index) {
			// Erase a level and re-check
			auto copy = report;
			copy.erase(copy.begin() + index);
			return safe_report(copy);
		});
	};

	return kg::sum(input | std::views::transform(dampened_safe_report));
}
