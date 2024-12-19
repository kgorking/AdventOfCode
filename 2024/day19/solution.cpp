export module aoc.day19;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(6, 16);
export constexpr auto expected_input = std::make_pair(233, 691316989225259ll);

std::int64_t count_combinations(std::string_view design, std::span<std::string_view const> patterns) {
	std::unordered_map<std::string_view, std::int64_t> cache;

	auto valid_combos = [&](this auto& self, std::string_view design) {
		// If we reached the end of the design string, return 1
		if (design.empty())
			return 1ll;

		// Return a potential cached value
		if (cache.contains(design))
			return cache[design];

		std::int64_t combos = 0;

		for (auto pattern : patterns)
			if (design.starts_with(pattern))
				combos += self(design.substr(pattern.size()));

		cache[design] = combos;
		return combos;
	};

	return valid_combos(design);
}

export auto part1(auto&& input) {
	auto const& [patterns, designs] = input;
	auto fn = std::bind_back(count_combinations, patterns);
	return kg::sum(designs | std::views::transform(fn) | std::views::transform([](auto i) { return i > 0; }));
}

export auto part2(auto&& input) {
	auto const& [patterns, designs] = input;
	auto fn = std::bind_back(count_combinations, patterns);
	return kg::sum(designs | std::views::transform(fn));
}
