import aoc;
import std;
#define DAY "day04"

// Holds the expected values for sample + input. Used in unit tests.
static constexpr auto expected_sample = std::make_pair(13, 43);
static constexpr auto expected_input = std::make_pair(1428, 8936);

constexpr auto neighbours_offsets = std::array<kg::pos2di, 8> { kg::pos2di
	{ -1, -1 }, { 0, -1 }, { 1, -1 },
	{ -1,  0 },            { 1,  0 },
	{ -1,  1 }, { 0,  1 }, { 1,  1 },
};

using matrix_t = kg::matrix_t<char, 137, 137>;

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
	using neighbourhood = std::array<char*, 8>;
	std::vector<neighbourhood> active_rolls;
	matrix_t roll_count {};
	kg::grid grid_roll { roll_count };

	// Stick pointers to paper rolls counts in a vector,
	// along with the pointers to the neighbouring positions.
	// This allows me to avoid grid lookups during processing.
	for (auto const pos : input | kg::views::coord2d) {
		if ('@' == input[pos.y][pos.x]) {
			neighbourhood n { };
			auto neighbour = n.data();

			for (kg::pos2di const offset : neighbours_offsets) {
				*neighbour = &grid_roll[pos + offset];
				**neighbour += 1;
				neighbour += 1;
			}

			active_rolls.push_back(n);
		}
	}

	int total_removed = 0;
	std::size_t active = active_rolls.size();
	while (true) {
		int const last_active = active;

		// Process all active rolls
		for (auto i = 0ull; i < active;) {
			auto& n = active_rolls[i];

			// Deduce the center position from the neighbour to the left
			auto const center = n[3] + 1;

			if (*center < 4) {
				// Decrease the roll count for all neighbouring positions
				for (int i = 0; i < 8; i++)
					*n[i] -= 1;

				// Remove the roll from the vector
				// by copying the last element into its place
				active -= 1;
				n = active_rolls[active];
			} else {
				// The roll remains in the vector
				i++;
			}
		}

		// Check if I'm done
		if (active == last_active) break;

		total_removed += (last_active - active);
	}

	return total_removed;
}

// Tests and benchmarks
#include "../tests_and_benchmarks.h"
