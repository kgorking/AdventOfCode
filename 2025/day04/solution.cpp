import aoc;
import std;
#define DAY "day04"

// Holds the expected values for sample + input. Used in unit tests.
static constexpr auto expected_sample = std::make_pair(13, 43);
static constexpr auto expected_input = std::make_pair(1428, 8936);

using matrix_t = kg::matrix_t<char, 138, 138>;

// When a roll is present at a position, count it for all adjacent positions
matrix_t count_adjacent_rolls(auto const& current_rolls) {
	constexpr auto neighbours_offsets = std::array<kg::pos2di, 8> { kg::pos2di
		{ -1, -1 }, { 0, -1 }, { 1, -1 },
		{ -1,  0 },            { 1,  0 },
		{ -1,  1 }, { 0,  1 }, { 1,  1 },
	};

	matrix_t mat {};

	kg::grid grid_mat { mat };

	for (int y = 0; y < current_rolls.size(); y++) {
		for (int x = 0; x < current_rolls[0].size(); x++) {
			if ('@' == current_rolls[y][x])
				for (kg::pos2di pos{ x, y }; kg::pos2di const offset : neighbours_offsets)
					grid_mat[pos + offset] += 1;
		}
	}
	return mat;
}

static auto part1(auto const& input) {
	matrix_t current_rolls {};
	for (int y = 1; y < input.size() - 1; y++) {
		for (int x = 1; x < input[0].size() - 1; x++) {
			if ('@' == input[y][x])
				current_rolls[y][x] = '@';
		}
	}

	matrix_t const roll_count = count_adjacent_rolls(current_rolls);

	int count = 0;
	for (int y = 1; y<input.size() - 1; y++) {
		for (int x = 1; x < input[0].size() - 1; x++) {
			bool const is_roll = '@' == current_rolls[y][x];
			count += is_roll && (roll_count[y][x] < 4);
		}
	}

	return count;
}

static auto part2(auto const& input) {
	using neighbourhood = std::array<char*, 4>; // 24 bytes for 9 positions. 8 bytes for padding.
	
	// Pre-alloc and pre-fault memory for active rolls. Saves ~50us
	int mem_needed = 0;
	for (int y = 1; y < input.size() - 1; y++) {
		for (int x = 1; x < input[0].size() - 1; x++)
			mem_needed += '@' == input[y][x];
	}
	std::vector<neighbourhood> active_rolls(mem_needed);

	// Stick pointers to paper rolls counts in a vector,
	// along with the pointers to the neighbouring positions.
	// This allows me to avoid grid lookups during processing.
	matrix_t roll_count {};
	std::size_t active = 0;

	for (int y = 1; y < input.size() - 1; y++) {
		for (int x = 1; x < input[0].size() - 1; x++) {
			if ('@' == input[y][x]) {
				neighbourhood& n = active_rolls[active++];

				// Get pointers to neighbouring positions
				auto const pos = kg::pos2di { x, y };
				n[0] = &roll_count[pos.y - 1][pos.x - 1];
				n[1] = &roll_count[pos.y    ][pos.x - 1];
				n[2] = &roll_count[pos.y + 1][pos.x - 1];

				// Increase the roll count for all neighbouring positions
				n[0][0] += 1; n[0][1] += 1; n[0][2] += 1;
				n[1][0] += 1;               n[1][2] += 1;
				n[2][0] += 1; n[2][1] += 1; n[2][2] += 1;
			}
		}
	}

	int total_removed = 0;
	std::size_t const initial_active = active;

	while (true) {
		std::size_t const last_active = active;

		// Process all active rolls
		for (std::size_t i = 0; i < active;) {
			neighbourhood const& n = active_rolls[i];

			auto const roll = n[1][1];
			if (roll < 4) {
				// Decrease the roll count for all neighbouring positions
				n[0][0] -= 1; n[0][1] -= 1; n[0][2] -= 1;
				n[1][0] -= 1;               n[1][2] -= 1;
				n[2][0] -= 1; n[2][1] -= 1; n[2][2] -= 1;

				// Remove the roll from the vector
				active_rolls[i] = active_rolls[--active];
			} else {
				// The roll remains in the vector
				i++;
			}
		}

		// No change means I'm done
		if (active == last_active)
			break;
	}

	return initial_active - active;
}

// Tests and benchmarks
#include "../tests_and_benchmarks.h"
