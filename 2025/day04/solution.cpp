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

using matrix_t = kg::matrix_t<char, 138, 138>;

// When a roll is present at a position, count it for all adjacent positions
matrix_t count_adjacent_rolls(auto const& current_rolls) {
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
	using neighbourhood = std::array<char*, 8>; // A delicious 64 bytes stride.
	std::vector<neighbourhood> active_rolls;
	matrix_t roll_count {};
	kg::grid grid_roll { roll_count };
	
	// Pre-alloc and pre-fault memory for active rolls. Saves ~100us
	int mem_needed = 0;
	for (int y = 1; y < input.size() - 1; y++) {
		for (int x = 1; x < input[0].size() - 1; x++)
			mem_needed += '@' == input[y][x];
	}
	active_rolls.resize(mem_needed);

	// Stick pointers to paper rolls counts in a vector,
	// along with the pointers to the neighbouring positions.
	// This allows me to avoid grid lookups during processing.
	//for (auto const pos : input | kg::views::coord2d) {
	std::size_t active = 0;
	for (int y = 1; y < input.size() - 1; y++) {
		for (int x = 1; x < input[0].size() - 1; x++) {
			if ('@' == input[y][x]) {
				neighbourhood n {};

				auto const pos = kg::pos2di { x, y };
				for (int i = 0; kg::pos2di const offset : neighbours_offsets) {
					n[i] = &grid_roll[pos + offset];
					*n[i] += 1;
					i += 1;
				}

				active_rolls[active++] = n;
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

			// Deduce the center position from the neighbour to its left.
			// This is only done to avoid an extra pointer in the neighbourhood array,
			// and to keep it the size of cache line.
			auto const center = n[3] + 1;

			if (*center < 4) {
				// Decrease the roll count for all neighbouring positions
				for (int i = 0; i < 8; i++)
					*n[i] -= 1;

				// Remove the roll from the vector
				active -= 1;
				active_rolls[i] = active_rolls[active];
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
