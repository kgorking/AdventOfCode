#include <array>
#include <iostream>
#include <stack>

using coord = std::pair<int8_t, int8_t>;

int main() {
	std::array<std::array<int8_t, 10>, 10> input{{
#include "input.txt"
	}};

	// Counts total number of flashes
	int num_flashes = 0;

	// Counts the number of octopuses that flashed in a single step
	int octo_flashed = 0;

	// Octos ready to pop
	std::stack<coord> flash;

	// Increases the energy level of a single octopus; adds to stack if it flashes
	auto const inc_octo_energy = [&](int x, int y) {
		input[y][x] += 1;

		if (input[y][x] == 10) {
			octo_flashed += 1;
			flash.emplace(x, y);
		}
	};

	// Run the steps
	for (int step = 1;; ++step) {
		octo_flashed = 0;

		// Increase energylevels
		for (int y = 0; y < 10; ++y) {
			for (int x = 0; x < 10; ++x) {
				// Reset octo energy if it flashed previous step
				if (input[y][x] > 9)
					input[y][x] = 1;
				else
					inc_octo_energy(x, y);
			}
		}

		// Propagate flashes
		while (!flash.empty()) {
			auto const [x, y] = flash.top();
			flash.pop();

			// count the flash
			num_flashes += 1;

			// Increase energy of neighbours, clamped to edges
			int const x1 = x - (x > 0);
			int const x2 = x + (x < 9);
			int const y1 = y - (y > 0);
			int const y2 = y + (y < 9);

			for (int j = y1; j <= y2; ++j) {	 // y-offset
				for (int i = x1; i <= x2; ++i) { // x-offset
					inc_octo_energy(i, j);
				}
			}
		}

		// Part 1
		if (100 == step) {
			std::cout << "Part 1: " << num_flashes << '\n';
		}

		// Part 2
		if (100 == octo_flashed) {
			std::cout << "Part 2: " << step << '\n';
			break;
		}
	}
}
