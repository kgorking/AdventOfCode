#include <iostream>
#include <array>
#include "input.h"

std::array<std::array<unsigned char, 1000>, 1000> lights;

int main() {
	for (auto const& op : input) {
		switch (op.cmd) {
			case command::on:
				for (int x = op.bottom_left.first; x <= op.top_right.first; ++x) {
					for (int y = op.bottom_left.second; y <= op.top_right.second; ++y) {
						lights[x][y]++;
					}
				}
				break;

			case command::off:
				for (int x = op.bottom_left.first; x <= op.top_right.first; ++x) {
					for (int y = op.bottom_left.second; y <= op.top_right.second; ++y) {
						lights[x][y] = std::max(0, lights[x][y] - 1);
					}
				}
				break;

			case command::toggle:
				for (int x = op.bottom_left.first; x <= op.top_right.first; ++x) {
					for (int y = op.bottom_left.second; y <= op.top_right.second; ++y) {
						lights[x][y] += 2;
					}
				}
				break;
		}
	}

	int active_lights = 0;
	for (auto light_row : lights)
		for (auto light : light_row)
			active_lights += light;
	std::cout << "total brightness: " << active_lights << '\n';
}
