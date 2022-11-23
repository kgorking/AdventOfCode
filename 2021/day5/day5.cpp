#include <array>
#include <bitset>
#include <iostream>

struct line {
	uint16_t x1, y1, x2, y2;

	bool vertical() const {
		return x1 == x2;
	}
	bool horizontal() const {
		return y1 == y2;
	}
	int x_direction() const {
		return (x1 < x2) ? 1 : -1;
	}
	int y_direction() const {
		return (y1 < y2) ? 1 : -1;
	}
};

constexpr std::array<line, 500> input{{
#include "input.txt"
}};

// ----

// Lines are filled into bits 0-999. Bits in 1000-1999 are true if more than one line touches the coord
using row = std::bitset<2000>;
std::array<row, 1000> field{};

void build_field(bool include_diagonals) {
	// reset field
	field.fill({});

	for (line const& l : input) {
		if (l.vertical()) {
			auto const y_step = l.y_direction();

			for (int y = l.y1; y != l.y2 + y_step; y += y_step) {
				field[y][l.x1 + 1000] = field[y][l.x1];
				field[y][l.x1] = true;
			}
		} else if (l.horizontal()) {
			auto const x_step = l.x_direction();

			for (int x = l.x1; x != l.x2 + x_step; x += x_step) {
				field[l.y1][x + 1000] = field[l.y1][x];
				field[l.y1][x] = true;
			}
		} else { // diagonal
			if (!include_diagonals)
				continue;

			auto const x_step = l.x_direction();
			auto const y_step = l.y_direction();

			for (int y = l.y1, x = l.x1; y != l.y2 + y_step && x != l.x2 + x_step; y += y_step, x += x_step) {
				field[y][x + 1000] = field[y][x];
				field[y][x] = true;
			}
		}
	}
}

size_t count_line_overlaps() {
	size_t overlaps = 0;

	for (row const& r : field) {
		overlaps += (r >> 1000).count();
	}

	return overlaps;
}

void part1() {
	build_field(false);
	std::cout << "Part 1: " << count_line_overlaps() << " overlaps found\n";
}

void part2() {
	build_field(true);
	std::cout << "Part 2: " << count_line_overlaps() << " overlaps found\n";
}

int main() {
	part1();
	part2();
}
