#include <array>
#include <bitset>
#include <iostream>
#include <vector>

constexpr std::array<int8_t, 100> draw_numbers{
#include "draw_numbers.txt"
};

struct number_index {
	uint8_t board;
	uint8_t column;
	uint8_t row;
};

// Contains data for all the boards and some helper functions
struct all_boards {
	std::array<int8_t, 5 * 5 * 100> values; // negative == marked
	std::bitset<100> completed;

	// Reads a value at the coordinates given
	int8_t peek(number_index const ni) const {
		return values[static_cast<size_t>(ni.board) * 25 + static_cast<size_t>(ni.column) * 5 + ni.row];
	}

	// Marks a value
	void mark(number_index const ni) {
		values[static_cast<size_t>(ni.board) * 25 + static_cast<size_t>(ni.column) * 5 + ni.row] |= 0x80;
	}

	// Sums up all the positive numbers
	int sum_unmarked_numbers(uint8_t const board) {
		int const offset = 25 * board;
		int sum = 0;
		for (int i = offset; i < offset + 25; ++i) {
			if (values[i] > 0)
				sum += values[i];
		}
		return sum;
	}

	// Sets the boards numbers to negative
	void mark_board_as_completed(uint8_t const board) {
		completed.set(board, true);
	}

	// Returns true if the board is won
	bool is_board_completed(uint8_t const board) const {
		return completed.test(board);
	}

	// Returns true if a full row or column is found
	bool check_if_won(number_index const ni) const {
		bool won = true;

		// check row
		for (uint8_t i = 0; i < 5 && won; i++) {
			won = won && (peek({ni.board, ni.column, i}) < 0);
		}

		// check column
		if (!won) {
			won = true;
			for (uint8_t i = 0; i < 5 && won; i++) {
				won = won && (peek({ni.board, i, ni.row}) < 0);
			}
		}

		return won;
	}

	// Sets all boards numbers back to positive
	void reset() {
		for (int8_t& val : values)
			val = std::abs(val);
	}
};

// --

// Values never go above 100
using number_lookup = std::array<std::vector<number_index>, 100>;

// Create a lookup table. Holds a value and its coordinates in each board that holds it
number_lookup build_lookup(all_boards const& boards) {
	number_lookup lookup;
	for (uint8_t board = 0; board < 100; ++board) {
		for (uint8_t column = 0; column < 5; ++column) {
			for (uint8_t row = 0; row < 5; ++row) {
				int8_t const board_value = boards.peek({board, column, row});
				lookup[board_value].emplace_back(board, column, row);
			}
		}
	}

	return lookup;
}

void part1(all_boards& input, number_lookup const& lookup) {
	// draw numbers and look for winning board
	for (int8_t const draw : draw_numbers) {
		for (number_index const ni : lookup.at(draw)) {
			input.mark(ni);

			// If the board is won, update the winner(s)
			if (input.check_if_won(ni)) {
				int const score = draw * input.sum_unmarked_numbers(ni.board);

				// correct score is 38913
				std::cout << "Part 1: score is " << score << '\n';
				return;
			}
		}
	}
}

void part2(all_boards& input, number_lookup const& lookup) {
	int last_board = -1;
	int last_draw = 0;

	// draw numbers and look for winning board
	for (int8_t const draw : draw_numbers) {
		for (number_index const ni : lookup.at(draw)) {
			if (input.is_board_completed(ni.board))
				continue;

			input.mark(ni);

			// If the board is won, update the winner(s)
			if (input.check_if_won(ni)) {
				input.mark_board_as_completed(ni.board);
				last_board = ni.board;
				last_draw = draw;
			}
		}
	}

	// correct score 16836
	int const score = last_draw * input.sum_unmarked_numbers(last_board);
	std::cout << "Part 2: score is " << score << '\n';
}

int main() {
	all_boards boards{
#include "boards.txt"
	};

	number_lookup const lookup = build_lookup(boards);

	part1(boards, lookup);

	boards.reset();
	part2(boards, lookup);
}
