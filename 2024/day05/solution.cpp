export module aoc.day05;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(143, 123);
export constexpr auto expected_input = std::make_pair(4662, 5900);

template <bool fix>
auto solution(auto&& input) {
	auto const& [rules, all_updates] = input;

	std::unordered_map<int, std::unordered_set<int>> order;
	for (auto [before, after] : rules)
		order[before].insert(after);

	auto sort_fn = [&order](int l, int r) { return order[l].contains(r); };

	if constexpr (!fix) {
		return kg::sum(all_updates | kg::views::filter_fn(std::ranges::is_sorted, sort_fn)
					   | std::views::transform([](auto const& v) { return v[v.size() / 2]; }));
	} else {
		return kg::sum(all_updates | kg::views::filter_fn(std::not_fn(std::ranges::is_sorted), sort_fn)
					   | std::views::transform([&](auto v) {
							 std::ranges::sort(v, sort_fn);
							 return v[v.size() / 2];
						 }));
	}
}

export auto part1(auto&& input) { return solution<false>(input); }
export auto part2(auto&& input) { return solution<true>(input); }