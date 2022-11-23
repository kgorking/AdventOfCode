#include <array>
#include <iostream>
#include <optional>

struct deterministic_die {
	int roll() {
		value += 1;
		rolls += 1;
		if (rolls > 100) {
			hundreds += 100;
			rolls -= 100;
		}
		return value;
	}

	int get_num_rolls() const {
		return hundreds + rolls;
	}

private:
	int rolls = 0;
	int hundreds = 0;
	int value = 0;
};

//           1 2 3
//          1  2  3
//         1   2   3
//        1    2    3
//       1     2     3
//      1      2      3
//  21=1       2=11    3=7
struct dirac_die {
	int roll() {
		value += 1;
		rolls += 1;
		return value;
	}

	int get_num_rolls() const {
		return rolls;
	}

private:
	int rolls = 0;
	int value = 0;
};

struct pawn {
	int space = 0;
	int score = 0;
};

struct game_stats {
	int dice_rolls;
	int winner_score;
	int loser_score;

	int get_puzzle_output() const {
		return loser_score * dice_rolls;
	}
};

template<typename Die>
struct game {
	game(int p1_start, int p2_start, int winning_score) : pawns{{{p1_start, 0}, {p2_start, 0}}}, winning_score(winning_score) {}

	// Run the game until a winner is found
	game_stats run() {
		do {
			auto const stats = turn();
			if (stats) {
				return stats.value();
			}
		} while (true);
	}

	// Do a turn of the game.
	// Returns a game_stats struct if the game was won
	std::optional<game_stats> turn() {
		pawn& current_pawn = pawns[player_turn];

		// On each player's turn, the player rolls the die three times and adds up the results
		int const roll_result = die.roll() + die.roll() + die.roll();

		// Then, the player moves their pawn that many times forward around the track
		// (that is, moving clockwise on spaces in order of increasing value, wrapping back around to 1 after 10).
		// So, if a player is on space 7 and they roll 2, 2, and 1, they would move forward 5 times,
		// to spaces 8, 9, 10, 1, and finally stopping on 2.
		current_pawn.space += roll_result;
		while (current_pawn.space > 10)
			current_pawn.space -= 10;

		// After each player moves, they increase their score by the value of the space their pawn stopped on
		current_pawn.score += current_pawn.space;

		// The game immediately ends as a win for any player whose score reaches at least 1000.
		if (current_pawn.score >= winning_score) {
			game_stats const stats {
				die.get_num_rolls(),
				pawns[player_turn].score,
				pawns[!player_turn].score
			};

			return stats;
		}

		player_turn = !player_turn;
		return {};
	}

	std::array<pawn, 2> pawns{};
	int winning_score = 0;
	bool player_turn = false;
	Die die;
	constexpr static int num_game_spaces_wrap = 11;
};

int main() {
	game<deterministic_die> example(4, 8, 1000);
	game_stats const example_stats = example.run();
	std::cout << "Winner score: " << example_stats.winner_score << '\n'; 
	std::cout << "Loser score: " << example_stats.loser_score << '\n'; 
	std::cout << "Dice rolls: " << example_stats.dice_rolls << '\n'; 
	std::cout << "Example: loser score * dice rolls = " << example_stats.get_puzzle_output() << '\n';

	// Part 1
	game<deterministic_die> part1(6, 10, 1000);
	game_stats const part1_stats = part1.run();
	std::cout << "Part 1: " << part1_stats.get_puzzle_output() << '\n';

	// Part 2
	game<dirac_die> part2(6, 10, 21);
	game_stats const part2_stats = part2.run();
	std::cout << "Part 2: " << part2_stats.get_puzzle_output() << '\n';
}
