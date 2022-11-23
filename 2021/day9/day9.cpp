#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <iostream>
#include <stack>
#include <string_view>
#include <vector>

int main() {
	// 100 x 100
	std::array<std::string, 100> input{
#include "input.txt"
	};

	// Part 1
	int risk_level = 0;
	std::vector<std::pair<int, int>> basins; // used in p2

	std::bitset<100> y_slope_up;
	std::bitset<100> y_slope_down;
	y_slope_up.flip();

	for (int y = 0; y < 100; ++y) {
		bool x_slope_l = true;

		for (int x = 0; x < 100; ++x) {
			bool const x_slope_r = (x < 99) ? (input[y][x] < input[y][x + 1]) : true;
			y_slope_down[x] = (y < 99) ? (input[y][x] < input[y + 1][x]) : true;

			bool const min_x = x_slope_l && x_slope_r;
			bool const min_y = y_slope_up[x] && y_slope_down[x];

			// test
			if (min_x && min_y) {
				risk_level += 1 + (input[y][x] - '0');
				basins.emplace_back(y, x);
			}

			// move
			x_slope_l = !x_slope_r;
		}

		y_slope_up = ~y_slope_down;
	}

	std::cout << "Part 1: " << risk_level << '\n';

	// Part 2
	auto const flood_fill = [&]<typename Coord>(Coord const start_coord) {
		// The count of tiles visited
		int count = 0;

		// Stack starts at lowest point from p1
		std::stack<Coord> stack;
		stack.push(start_coord);

		while (!stack.empty()) {
			auto const [y, x] = stack.top();
			stack.pop();

			if ('9' == input[y][x])
				continue;

			// Location is valid, so count it and invalidate it
			count += 1;
			input[y][x] = '9';

			// Add valid neighbours to the stack
			if (x > 0 && '9' != input[y][x - 1])
				stack.push({y, x - 1});
			if (x < 99 && '9' != input[y][x + 1])
				stack.push({y, x + 1});
			if (y > 0 && '9' != input[y - 1][x])
				stack.push({y - 1, x});
			if (y < 99 && '9' != input[y + 1][x])
				stack.push({y + 1, x});
		}

		return count;
	};

	std::array<int, 3> basin_sizes{0, 0, 0};
	for (auto basin : basins) {
		int const size = flood_fill(basin);

		auto const it = std::ranges::min_element(basin_sizes);
		if (*it < size)
			*it = size;
	}

	std::cout << "Part 2: " << basin_sizes[0] * basin_sizes[1] * basin_sizes[2] << '\n';
}
