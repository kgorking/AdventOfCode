import aoc;
import std;
#define DAY "day04"

// Holds the expected values for sample + input. Used in unit tests.
static constexpr auto expected_sample = std::make_pair(13, 43);
static constexpr auto expected_input = std::make_pair(1428, 8936);

constexpr auto neighbours_offsets = std::array<kg::pos2di, 8> {
	kg::pos2di { -1, -1 }, kg::pos2di { 0, -1 }, kg::pos2di { 1, -1 },
	kg::pos2di { -1,  0 },                       kg::pos2di { 1,  0 },
	kg::pos2di { -1,  1 }, kg::pos2di { 0,  1 }, kg::pos2di { 1,  1 },
};

using matrix_t = kg::matrix_t<char, 136, 136>;

// When a roll is present at a position, count it for all adjacent positions
matrix_t count_adjacent_rolls(auto const& current_rolls) {
	matrix_t mat {};

	kg::grid grid { current_rolls };
	kg::grid grid_mat { mat };

	for (kg::pos2di const pos : grid.coords())
		if ('@' == grid[pos])
			for (kg::pos2di const offset : neighbours_offsets)
				grid_mat[pos + offset] += 1;

	return mat;
}

static auto part1(auto const& input) {
	matrix_t current_rolls {};
	for (auto pos : input | kg::views::coord2d) {
		if ('@' == input[pos.y][pos.x])
			current_rolls[pos.y][pos.x] = '@';
	}

	matrix_t const roll_count = count_adjacent_rolls(current_rolls);

	int count = 0;
	for (int y=0; y<input.size(); y++) {
		for (int x = 0; x < input[0].size(); x++) {
			bool const is_roll = '@' == current_rolls[y][x];
			count += is_roll && (roll_count[y][x] < 4);
		}
	}

	return count;
}

static auto part2(auto const& input) {
	using neighbourhood = std::array<char*, 9>;
	std::vector<neighbourhood> active_rolls;
	matrix_t roll_count {};
	kg::grid grid_roll { roll_count };

	// Stick pointers to paper rolls counts in a vector,
	// along with the pointers to the neighbouring positions.
	// This allows me to avoid grid lookups during processing.
	for (auto pos : input | kg::views::coord2d) {
		if ('@' == input[pos.y][pos.x]) {
			neighbourhood n { &grid_roll[pos] };
			auto neighbours = n.data() + 1;

			for (kg::pos2di const offset : neighbours_offsets) {
				*neighbours = &grid_roll[pos + offset];
				**neighbours += 1;
				neighbours += 1;
			}

			active_rolls.push_back(n);
		}
	}

	int total_removed = 0;
	while (true) {
		int removed = 0;

		// Process all active rolls
		for (int i = 0; i < active_rolls.size();) {
			auto& n = active_rolls[i];

			if (*n[0] < 4) {
				// Decrease the roll count for all neighbouring positions
				for (int i=1; i < 9; i++)
					*(n[i]) -= 1;

				// Remove the roll from the vector
				// by swapping with the last element
				std::swap(n, active_rolls.back());
				active_rolls.pop_back();
				removed += 1;
			} else {
				// The roll remains on the vector
				i++;
			}
		}

		// Check if I'm done
		if (!removed) break;

		total_removed += removed;
	}

	return total_removed;
}

// Tests and benchmarks
#include "../tests_and_benchmarks.h"
