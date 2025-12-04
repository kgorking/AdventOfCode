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

using floor_t = kg::matrix_t<char, 136, 136>;

// When a roll is present at a position, count it for all adjacent positions
floor_t count_adjacent_rolls(auto const& current_rolls) {
	kg::grid grid { current_rolls };

	floor_t mat {};
	for (auto pos : grid.coords()) {
		if ('@' != grid[pos])
			continue;

		for (auto offset : neighbours_offsets) {
			auto npos = pos + offset;
			if (grid.in_bounds(npos) && '@' == grid[npos]) {
				mat[npos.y][npos.x] += 1;
			}
		}
	}
	return mat;
}

static auto part1(auto const& input) {
	floor_t current_rolls {};
	for (auto pos : input | kg::views::coord2d) {
		if ('@' == input[pos.y][pos.x])
			current_rolls[pos.y][pos.x] = '@';
	}

	floor_t const roll_count = count_adjacent_rolls(current_rolls);

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
	std::unordered_set<kg::pos2di> active_rolls;
	for (auto pos : input | kg::views::coord2d) {
		if ('@' == input[pos.y][pos.x]) {
			active_rolls.insert(pos);
		}
	}

	int total_removed = 0;
	floor_t roll_count = count_adjacent_rolls(input);
	kg::grid grid { input };
	std::vector<kg::pos2di> to_remove;

	while (true) {
		for (auto pos : active_rolls) {
			if (roll_count[pos.y][pos.x] < 4)
				to_remove.push_back(pos);
		}

		if (to_remove.empty())
			break;

		total_removed += to_remove.size();

		do {
			auto const pos = to_remove.back();
			active_rolls.erase(pos);
			to_remove.pop_back();

			for (auto offset : neighbours_offsets) {
				auto npos = pos + offset;
				if (grid.in_bounds(npos) && '@' == grid[npos]) {
					roll_count[npos.y][npos.x] -= 1;
				}
			}
		} while (!to_remove.empty());
	}

	return total_removed;
}

// Tests and benchmarks
#include "../tests_and_benchmarks.h"
