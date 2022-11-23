#include <array>
#include <iostream>
#include <ranges>
#include <unordered_set>

enum class FoldDir { x, y };

using packed_dot = uint32_t;
using dot = std::pair<uint16_t, uint16_t>;
using fold = std::pair<FoldDir, uint16_t>;

struct puzzle_input {
	std::array<dot, 815> dots;
	std::array<fold, 12> folds;
};

constexpr puzzle_input input{
#include "input.txt"
};

dot fold_dot(dot d, std::ranges::range auto const& folds) {
	for (fold f : folds) {
		switch (f.first) {
		case FoldDir::x:
			if (d.first > f.second)
				d.first = f.second * 2 - d.first;
			break;

		case FoldDir::y:
			if (d.second > f.second)
				d.second = f.second * 2 - d.second;
			break;
		}
	}

	return d;
}

int main() {
	// Part 1
	std::unordered_set<packed_dot> folded_dots;
	for (dot d : input.dots) {
		auto const [x, y] = fold_dot(d, input.folds | std::views::take(1));
		folded_dots.insert((y << 16) | x);
	}

	std::cout << "Part 1: " << folded_dots.size() << '\n';

	// Part 2

	// Create an 8x60 display and fill it in
	std::array<std::array<char, 60>, 8> display;
	for (auto& arr : display)
		arr.fill(' ');

	for (dot d : input.dots) {
		auto const [x, y] = fold_dot(d, input.folds);
		display[y][x] = '\xDB';
	}

	std::cout << "Part 2:\n";
	for (int y = 0; y < display.size(); ++y) {
		std::cout.write(display[y].data(), display[y].size()) << '\n';
	}
}
