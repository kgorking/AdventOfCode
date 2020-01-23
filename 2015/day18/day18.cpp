#include <iostream>
#include <array>
#include "input.h"

int constexpr Dimensions = 100;

struct packed_char {
	unsigned char state : 6;
	unsigned char num_neighbours : 2; // pack the neighbour count in the upper 2 bits
};

packed_char& get_packed_char(int x, int y) {
	return reinterpret_cast<packed_char&>(lights[y][x]);
}

bool is_light_on(int x, int y) {
	if (x < 0 || y < 0 || x >= Dimensions || y >= Dimensions)
		return false;
	return ('#' == get_packed_char(x,y).state);
}

void set_state(int x, int y, bool on) {
	if (x < 0 || y < 0 || x > Dimensions || y > Dimensions)
		return;
	get_packed_char(x, y).state = on ? '#' : '.';
}

void set_num_neighbours(int x, int y, int neighbours) {
	get_packed_char(x, y).num_neighbours = (neighbours > 3) ? 0 : neighbours;
}

void count_neighbours() {
	for (int y = 0; y < Dimensions; y++) {
		for (int x = 0; x < Dimensions; x++) {
			int const neighbours_on =
				is_light_on(x - 1, y - 1) +
				is_light_on(x, y - 1) +
				is_light_on(x + 1, y - 1) +
				is_light_on(x - 1, y) +
				is_light_on(x + 1, y) +
				is_light_on(x - 1, y + 1) +
				is_light_on(x, y + 1) +
				is_light_on(x + 1, y + 1);
			set_num_neighbours(x, y, neighbours_on);
		}
	}
}

void update_state() {
	for (int y = 0; y < Dimensions; y++) {
		for (int x = 0; x < Dimensions; x++) {
			// Dont alter the corners (part 2)
			if ((x == 0 || x == Dimensions - 1) && (y == 0 || y == Dimensions - 1))
				continue;

			auto & pc = get_packed_char(x, y);
			if (pc.state == '#') {
				if (pc.num_neighbours < 2)
					pc.state = '.';
			}
			else {
				if (pc.num_neighbours == 3)
					pc.state = '#';
			}

			pc.num_neighbours = 0;
		}
	}
}

void print_lights() {
	for (int y = 0; y < Dimensions; y++) {
		for (int x = 0; x < Dimensions; x++) {
			std::cout << get_packed_char(x, y).state;
		}
		std::cout << '\n';
	}
}

int count_active_lights() {
	int total = 0;
	for (int y = 0; y < Dimensions; y++) {
		for (int x = 0; x < Dimensions; x++) {
			total += is_light_on(x, y);
		}
	}
	return total;
}

int main() {
	for (int i = 0; i < 100; i++) {
		count_neighbours();
		update_state();
	}
	std::cout << "Active lights after 100 iterations: " << count_active_lights() << '\n';
}
