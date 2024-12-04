export module aoc.day04;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(18, 9);
export constexpr auto expected_input = std::make_pair(2406, 1807);

using ipair = std::pair<int, int>;
constexpr std::string_view search_word{"MAS"};

template <bool X, int N>
int xmas_count(std::array<std::string_view, N> input, char const c, std::span<const ipair> directions, std::span<const int, 3> offsets) {
	int num_found = 0;

	for (auto const [sy, line] : input | std::views::enumerate) {
		for (std::size_t sx = line.find(c); sx != std::string_view::npos; sx = line.find(c, sx + 1)) {
			int const directions_found = kg::sum(directions | std::views::transform([&](ipair dir) {
				auto const offset_to_char = std::views::transform([&](int offset) {
					int const x = sx + (dir.first * offset);
					int const y = sy + (dir.second * offset);
					return (x >= 0 && x < input[y].size() && y >= 0 && y < input.size()) ? input[y][x] : '.';
				});

				return std::ranges::equal(search_word, offsets | offset_to_char);
			}));

			if (X) {
				// Only count it as found if the word exists in two directions
				num_found += (directions_found == 2);
			} else {
				num_found += directions_found;
			}
		}
	}

	return num_found;
}

export auto part1(auto const& input) {
	auto const directions = std::to_array<ipair>({
		/* right */ { 1,  0},
		/* left  */ {-1,  0},
		/* down  */ { 0,  1},
		/* up    */ { 0, -1},
		/* d r   */ { 1,  1},
		/* d l   */ {-1,  1},
		/* u r   */ { 1, -1},
		/* u l   */ {-1, -1}});

	std::array const offsets{1, 2, 3};

	return xmas_count<false>(input, 'X', directions, offsets);
}

export auto part2(auto const& input) {
	auto const directions = std::to_array<ipair>({
		/* d r */ { 1,  1},
		/* d l */ {-1,  1},
		/* u r */ { 1, -1},
		/* u l */ {-1, -1}});

	std::array const offsets{-1, 0, 1};

	return xmas_count<true>(input, 'A', directions, offsets);
}
