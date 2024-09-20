#include <array>
#include <iostream>
#include <ranges>
#include <unordered_set>

enum FoldDir { x, y };

using packed_dot = uint32_t;
using dot = std::array<uint16_t, 2>;
using fold = std::pair<FoldDir, uint16_t>;

struct puzzle_input {
	std::array<dot, 815> dots;
	std::array<fold, 12> folds;
};

constexpr puzzle_input input{
#include "input.txt"
};

dot fold_dot(dot d, std::ranges::range auto const& folds) {
	for (auto const [dir, size] : folds) {
		if (d[dir] > size)
			d[dir] = size * 2 - d[dir];
	}

	return d;
}

void part1() {
	std::unordered_set<packed_dot> folded_dots;
	for (dot d : input.dots) {
		auto const [x, y] = fold_dot(d, input.folds | std::views::take(1));
		folded_dots.insert((y << 16) | x);
	}

	std::cout << "Part 1: " << folded_dots.size() << '\n';
}

void part2() {
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

int main() {
	part1();
	part2();
}
