#include <array>
#include <iomanip>
#include <iostream>
#include <queue>

constexpr int Dim = 100;
constexpr std::array<std::array<int8_t, Dim>, Dim> input{
#include "input.txt"
};

int calc_risk(int y, int x) {
	int const extra_risk = (y / Dim) + (x / Dim);
	int const total_risk = extra_risk + input[y % Dim][x % Dim];
	return 1 + ((total_risk - 1) % 9);
}

int find_path(int repeats = 1) {
	int const height = repeats * Dim;
	int const width = repeats * Dim;

	std::vector<int> accumulated_risk_levels(width * height, std::numeric_limits<int>::max());

	// Helper-lambda to update a neighbour
	auto const update_neighbour = [&](int j, int i, int acc_current_risk) -> int {
		int const neighbour_risk = calc_risk(j, i);
		int const combined_risk = acc_current_risk + neighbour_risk;

		auto& acc_neighbour_risk = accumulated_risk_levels[j * height + i];
		if (combined_risk < acc_neighbour_risk) {
			acc_neighbour_risk = combined_risk;
		}

		return acc_current_risk;
	};

	// Set the initial risk level
	accumulated_risk_levels[0] = 0;

	// Process the tiles
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// the current accumulated risk level
			auto& acc_current_risk = accumulated_risk_levels[y * height + x];

			// Update neighbours forward along x,y
			if (x < width - 1) {
				update_neighbour(y, x + 1, acc_current_risk);
			}
			if (y < height - 1) {
				update_neighbour(y + 1, x, acc_current_risk);
			}
		}
	}

	for (int y = height - 1; y >= 0; --y) {
		for (int x = width - 1; x >= 0; --x) {
			// the current accumulated risk level
			auto& acc_current_risk = accumulated_risk_levels[y * height + x];

			// Propagate changes to neighbours backwards along -x,-y
			if (y > 0)
				update_neighbour(y - 1, x, acc_current_risk);
			if (x > 0)
				update_neighbour(y, x - 1, acc_current_risk);
		}
	}

	return accumulated_risk_levels.back();
}

int main() {
	// Part 1
	int const risk_level_p1 = find_path();
	std::cout << "Part 1: " << risk_level_p1 << '\n';

	int const risk_level_p2 = find_path(5);
	std::cout << "Part 2: " << risk_level_p2 << '\n';
}
