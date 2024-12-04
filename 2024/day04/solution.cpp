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
			int const directions_found =
				kg::sum(directions | std::views::transform([&](ipair dir) {
					// Check if a word exists in the direction
					return std::ranges::equal(search_word, offsets | std::views::transform([&](int offset) {
						// Convert an offset to a character in the input
						int const x = sx + (dir.first * offset);
						int const y = sy + (dir.second * offset);
						return (y >= 0 && y < input.size() && x >= 0 && x < input[y].size())
									? input[y][x]
									: '\0';
					}));
				}));

			num_found += X ? (directions_found == 2) : directions_found;
		}
	}

	return num_found;
}

export auto part1(auto const& input) {
	return xmas_count<false>(input, 'X', std::to_array<ipair>({{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}}), std::array{1, 2, 3});
}

export auto part2(auto const& input) {
	return xmas_count<true>(input, 'A', std::to_array<ipair>({{1, 1}, {-1, 1}, {1, -1}, {-1, -1}}), std::array{-1, 0, 1});
}