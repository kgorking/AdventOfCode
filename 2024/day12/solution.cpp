export module aoc.day12;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(1930, 1206);
export constexpr auto expected_input = std::make_pair(1549354, 937032);


export auto part1(auto&& input) {
	auto grid = kg::grid { input }; // wrap the input as a grid
	auto vis = kg::visited { grid }; // create visited matrix from grid size
	auto stack = std::vector<kg::pos2di> {};

	auto const flood_fill = [&](std::pair<char, kg::pos2di> const start) {
		int fences = 0;
		int area = 0;

		// Stack starts at p
		stack.push_back(start.second);

		while (!stack.empty()) {
			kg::pos2di const p = stack.back();
			stack.pop_back();

			// Check if at a boundary between plots
			if (grid[p] != start.first) {
				fences += 1;
				continue;
			}

			// Ignore if this position has already been visited
			if (vis.test_or_set(p))
				continue;

			// Increase the area
			area += 1;

			// Add neighbours to the stack
			p.add_neighbours_to(stack);
		}

		return area * fences;
	};

	return kg::sum(grid.values_and_coords()
		| std::views::filter([&](auto p) { return !vis.test(p.second); })
		| std::views::transform(flood_fill));
}

export auto part2(auto&& input) {
	struct directional_info {
		bool x:1, positive:1;
	};

	auto grid = kg::grid { input }; // wrap the input as a grid
	auto vis = kg::visited(grid);	// create visited matrix from grid size
	auto x_fences = std::vector<std::vector<short>>(1 + 2 * input.size());
	auto y_fences = std::vector<std::vector<short>>(1 + 2 * input.size());
	auto stack = std::vector<std::pair<kg::pos2di, directional_info>> {};

	auto const flood_fill = [&](std::pair<char, kg::pos2di> const start) {
		int area = 0;

		// Stack starts at p
		stack.push_back({ start.second, { 1, 0 } });

		while (!stack.empty()) {
			auto const [p, info] = stack.back();
			stack.pop_back();

			// Check if I'm at a boundary between plots
			if (grid[p] != start.first) {
				// The 'info.positive' value is used to distinguish between closely positioned plots. fx:
				//
				// 0.  AAAAA
				// 1.  BBBBB
				// 2.  AAAAA
				//
				// Here, the two A rows would both have fences running along
				// y=1, so one of them is offset by the size of the input to
				// create distinct values in the fences map

				int const offset = input.size() * info.positive;
				if (info.x)
					x_fences[1 + p.x].push_back(p.y + offset);
				else
					y_fences[1 + p.y].push_back(p.x + offset);
				continue;
			}

			// Ignore if this position has already been visited
			if (vis.test_or_set(p))
				continue;

			// Increase the area
			area += 1;

			// Add neighbours to the stack
			stack.push_back({ { p.x - 1, p.y }, { true, false } });
			stack.push_back({ { p.x + 1, p.y }, { true, true } });
			stack.push_back({ { p.x, p.y - 1 }, { false, false } });
			stack.push_back({ { p.x, p.y + 1 }, { false, true } });
		}

		// sorted vec: 1 2 3   5 6   21 22 34
		auto find_fence_holes = std::views::pairwise_transform([](short a, short b) { return b - a != 1; });

		auto count_sides
			= std::views::filter(std::not_fn(&std::vector<short>::empty))
			| std::views::transform([&](std::vector<short>& vec) {
				std::ranges::sort(vec);
				auto const result = 1 + kg::sum(vec | find_fence_holes);
				vec.clear();
				return result;
				});

		int const x_sides = kg::sum(x_fences | count_sides);
		int const y_sides = kg::sum(y_fences | count_sides);
		int const result = area * (x_sides + y_sides);
		return result;
	};

	return kg::sum(grid.values_and_coords()
		| std::views::filter([&](auto p) { return !vis.test(p.second); })
		| std::views::transform(flood_fill));
}
