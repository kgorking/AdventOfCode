export module aoc.day04;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(18, 9);
export constexpr auto expected_input = std::make_pair(2406, 1807);

template <bool X, int N>
int xmas_count(std::array<std::string_view, N> input, char const c, std::span<const kg::pos2di> directions, std::span<const int, 3> offsets) {
	return kg::sum(kg::views::indexed_transform(input, [&](int sy, std::string_view line) {
		return kg::sum(kg::views::indexed_transform(line, [&](int sx, char v) {
			return (c != v) ? 0 : kg::sum(directions | std::views::transform([&](kg::pos2di dir) {
				auto word_in_dir = offsets | std::views::transform([&](int offset) {
					kg::pos2di const p = kg::pos2di(sx, sy) + (dir * offset);
					return (p.y >= 0 && p.y < input.size() && p.x >= 0 && p.x < input[p.y].size()) ? input[p.y][p.x] : '\0';
				});
				return std::ranges::equal(std::string_view { "MAS" }, word_in_dir);
			}));
		}) | std::views::transform([](int v) { return X ? (v == 2) : v; }));
	}));
}

export auto part1(auto const& input) {
	return xmas_count<false>(input, 'X',
		std::to_array<kg::pos2di>({ { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } }),
		std::array { 1, 2, 3 });
}

export auto part2(auto const& input) {
	return kg::sum(input
		| kg::views::matrix<3, 3>
		| kg::views::filter_eq('A', 1, 1)
		| std::views::transform([](auto const& m) {
			return
				6 == std::abs(m[0][0] - m[2][2]) &&
				6 == std::abs(m[0][2] - m[2][0]); })
	);
}