import aoc;
import std;
#define DAY "day04"

// Holds the expected values for sample + input. Used in unit tests.
static constexpr auto expected_sample = std::make_pair(13, 43);
static constexpr auto expected_input = std::make_pair(1428, 8936);

using matrix_t = kg::matrix_t<char, 138, 138>;

static auto part1(auto const& input) {
	constexpr auto neighbours_offsets = std::array<kg::pos2di, 8> { kg::pos2di
		{ -1, -1 }, { 0, -1 }, { 1, -1 },
		{ -1,  0 },            { 1,  0 },
		{ -1,  1 }, { 0,  1 }, { 1,  1 },
	};

	// When a roll is present at a position, count it for all adjacent positions
	matrix_t roll_count {};
	for (int y = 1; y < input.size() - 1; y++) {
		for (int x = 1; x < input[0].size() - 1; x++) {
			if ('@' == input[y][x])
				for (kg::pos2di pos { x, y }; kg::pos2di const offset : neighbours_offsets)
					roll_count[pos.y + offset.y][pos.x + offset.x] += 1;
		}
	}

	int count = 0;
	for (int y = 1; y<input.size() - 1; y++) {
		for (int x = 1; x < input[0].size() - 1; x++) {
			bool const is_roll = '@' == input[y][x];
			count += is_roll && (roll_count[y][x] < 4);
		}
	}

	return count;
}

static auto part2(auto const& input) {
	// Stick pointers to paper rolls counts in an array,
	// along with the pointers to the neighbouring positions.
	// This allows me to avoid grid lookups during processing.
	using neighbourhood = std::array<char*, 3>; // 24 bytes to store a paper rolls entire neighbourhood.
	std::array<neighbourhood, 13000> active_rolls{};
	matrix_t roll_counts {};

	// Set up the initial roll counts and active rolls
	int active = 0;
	{
		for (int y = 1; y < input.size() - 1; y++) {
			for (int x = 1; x < input[0].size() - 1; x++) {
				if ('@' == input[y][x]) {
					neighbourhood& n = active_rolls[active++];

					// Get pointers to neighbouring positions
					n[0] = &roll_counts[y - 1][x - 1];
					n[1] = &roll_counts[y    ][x - 1];
					n[2] = &roll_counts[y + 1][x - 1];

					// Increase the roll count for all neighbouring positions
					n[0][0] += 1; n[0][1] += 1; n[0][2] += 1;
					n[1][0] += 1;               n[1][2] += 1;
					n[2][0] += 1; n[2][1] += 1; n[2][2] += 1;
				}
			}
		}
	}

	// Process active rolls until no more can be removed
	int const initial_active = active;
	int last_active;

	do {
		last_active = active;
		for (int i = 0; i < active;) {
			neighbourhood const& n = active_rolls[i];

			char const num_roll_neighbours = n[1][1];
			if (num_roll_neighbours < 4) {
				// Decrease the roll count for all neighbouring positions
				n[0][0] -= 1; n[0][1] -= 1; n[0][2] -= 1;
				n[1][0] -= 1;               n[1][2] -= 1;
				n[2][0] -= 1; n[2][1] -= 1; n[2][2] -= 1;
			
				// Remove the roll from consideration
				active_rolls[i] = active_rolls[--active];
			} else {
				// Next roll
				i++;
			}
		}
	} while (active != last_active);

	return initial_active - active;
}

// Tests and benchmarks
#include "../tests_and_benchmarks.h"
