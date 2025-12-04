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
	std::queue<kg::pos2di> active_rolls;
	matrix_t roll_count {};
	kg::grid grid_roll { roll_count };

	// Stick paper rolls positions in a stack,
	// and find the roll counts of neighbouring positions.
	for (auto pos : input | kg::views::coord2d) {
		if ('@' == input[pos.y][pos.x]) {
			active_rolls.push(pos);
			for (kg::pos2di const offset : neighbours_offsets)
				grid_roll[pos + offset] += 1;
		}
	}

	int total_removed = 0;
	while (true) {
		int removed = 0;
		int active = active_rolls.size();

		// Process all active rolls
		while (active--) {
			auto const pos = active_rolls.front();
			active_rolls.pop();

			if (grid_roll[pos] < 4) {
				removed += 1;

				// Decrease the roll count for all neighbouring positions
				for (auto offset : neighbours_offsets)
					grid_roll[pos + offset] -= 1;
			} else {
				// The roll goes back on the stack
				active_rolls.push(pos);
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
