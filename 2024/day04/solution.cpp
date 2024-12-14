export module aoc.day04;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(18, 9);
export constexpr auto expected_input = std::make_pair(2406, 1807);

constexpr auto directions
	= std::to_array<kg::pos2di>({ { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } });

export auto part1(auto const& input) {
	auto count_xmas_in_all_directions
		= input
		| kg::views::with_coord2d
		| kg::views::filter_equal('X', kg::select<0>)
		| std::views::values
		| std::views::transform([&](kg::pos2di p) {
				return kg::sum(directions | std::views::transform([&](kg::pos2di dir) {
					auto offset_to_char_in_input = [&](int offset) {
						kg::pos2di const w = p + (dir * offset);
						return kg::at(input, w, '\0');
					};
					auto word_in_dir = std::views::iota(1,4) | std::views::transform(offset_to_char_in_input);
					return std::ranges::equal(std::string_view { "MAS" }, word_in_dir);
				}));
			});
	return kg::sum(count_xmas_in_all_directions);
}

export auto part2(auto const& input) {
	return kg::sum(input
		| kg::views::matrix<3, 3>
		| kg::views::filter_equal('A', kg::index<1, 1>{})
		| std::views::transform([](kg::matrix_t<char,3,3> const& m) {
			return
				6 == std::abs(m[0][0] - m[2][2]) &&
				6 == std::abs(m[0][2] - m[2][0]); })
	);
}