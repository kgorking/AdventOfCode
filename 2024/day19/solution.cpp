export module aoc.day19;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(6, 16);
export constexpr auto expected_input = std::make_pair(233, 691316989225259ll);

std::int64_t count_combinations(std::string_view design, std::span<std::string_view const> patterns) {
	std::unordered_map<std::string_view, std::int64_t> cache;

	auto valid_combos = [&](this auto& valid_combos, std::string_view design) {
		if (design.empty())
			return 1ll;

		if (cache.contains(design))
			return cache[design];

		std::int64_t combos = 0;

		for (auto pattern : patterns)
			if (design.starts_with(pattern))
				combos += valid_combos(design.substr(pattern.size()));

		return cache[design] = combos;
	};

	return valid_combos(design);
}

export auto part1(auto&& input) {
	auto const& [patterns, designs] = input;
	auto fn = [&](auto design) { return count_combinations(design, patterns) > 0; };
	return kg::sum(designs | std::views::transform(fn));
}

export auto part2(auto&& input) {
	auto const& [patterns, designs] = input;
	auto fn = std::bind_back(count_combinations, patterns);
	return kg::sum(designs | std::views::transform(fn));
}
