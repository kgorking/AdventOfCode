#include <array>
#include <iostream>

enum class Direction { forward, up, down };

struct move {
	Direction direction;
	int amount;
};

constexpr std::array<move, 1000> input{{
#include "input.txt"
}};

struct submarine {
	int location = 0;
	int depth = 0;

	void do_move(move const m) {
		switch (m.direction) {
		case Direction::forward:
			location += m.amount;
			break;

		case Direction::up:
			depth -= m.amount;
			break;

		case Direction::down:
			depth += m.amount;
			break;
		}
	}
};

struct submarine_p2 {
	int location = 0;
	int depth = 0;
	int aim = 0;

	void do_move(move const m) {
		switch (m.direction) {
		case Direction::forward:
			location += m.amount;
			depth += aim * m.amount;
			break;

		case Direction::up:
			depth -= m.amount;
			break;

		case Direction::down:
			aim += m.amount;
			break;
		}
	}
};

void part1() {
	submarine sub;

	for (move const m : input) {
		sub.do_move(m);
	}

	std::cout << "Part 1: " << sub.depth * sub.location << '\n';
}

void part2() {
	submarine_p2 sub;

	for (move const m : input) {
		sub.do_move(m);
	}

	std::cout << "Part 2: " << sub.depth * sub.location << '\n';
}

int main() {
	part1();
	part2();
}
