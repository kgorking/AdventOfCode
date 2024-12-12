export module aoc.day12;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(1930, 1206);
export constexpr auto expected_input = std::make_pair(1549354, 937032);

export auto part1(auto&& input) {
	kg::visited vis(input.size(), input[0].size());

	auto const in_bounds = [&](kg::pos2di const p) { return (p.x >= 0 && p.y >= 0 && p.y < input.size() && p.x < input[0].size()); };
	auto const flood_fill = [&](kg::pos2di const p) {
		int fences = 0;
		int area = 0;
		char const val = input[p.y][p.x];

		// Stack starts at p
		std::vector<kg::pos2di> stack;
		stack.push_back(p);

		while (!stack.empty()) {
			auto const p = stack.back();
			stack.pop_back();

			// Check if I'm at a boundary between plots
			if (!in_bounds(p) || input[p.y][p.x] != val) {
				fences += 1;
				continue;
			}

			// Ignore if this position has already been visited
			if (vis.test_or_set(p))
				continue;

			// Increase the area
			area += 1;

			// Add neighbours to the stack
			stack.push_back({ p.x - 1, p.y });
			stack.push_back({ p.x + 1, p.y });
			stack.push_back({ p.x, p.y - 1 });
			stack.push_back({ p.x, p.y + 1 });
		}

		return area * fences;
	};

	return kg::sum(input
		| kg::views::coord2d
		| std::views::values
		| std::views::filter([&](auto p) { return !vis.test(p); })
		| std::views::transform(flood_fill));
}

export auto part2(auto&& input) {
	kg::visited vis(input.size(), input[0].size());

	std::unordered_map<int, std::vector<int>> x_fences, y_fences;

	auto const in_bounds = [&](kg::pos2di const p) { return (p.x >= 0 && p.y >= 0 && p.y < input.size() && p.x < input[0].size()); };
	auto const flood_fill = [&](kg::pos2di const p) {
		int area = 0;
		kg::pos2di last_p = p;
		char const val = input[p.y][p.x];

		// directional info
		struct directional_info {
			bool x:1, positive:1;
		};

		// Stack starts at p
		std::vector<std::pair<kg::pos2di, directional_info>> stack;
		stack.push_back({ p, { 1, 0 } });

		while (!stack.empty()) {
			auto const [p, info] = stack.back();
			stack.pop_back();

			// Check if I'm at a boundary between plots
			if (!in_bounds(p) || input[p.y][p.x] != val) {
				// The 'info.positive' value is used to distinguish between closely positioned plots. fx:
				//
				// 0.  AAAAA
				// 1.  BBBBB
				// 2.  AAAAA
				//
				// Here, the two A rows would both have fences running along
				// y=1, so one them is offset by the size of the input to
				// create distinct values in the fences map

				if (info.x)
					x_fences[p.x].push_back(p.y + input.size() * info.positive);
				else
					y_fences[p.y].push_back(p.x + input.size() * info.positive);
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

		auto find_fence_holes = std::views::pairwise_transform([](int a, int b) { return b - a != 1; });

		auto count_sides
			= std::views::values
			| std::views::filter(std::not_fn(&std::vector<int>::empty))
			| std::views::transform([&](std::vector<int>& vec) {
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

	return kg::sum(input
		| kg::views::coord2d
		| std::views::values
		| std::views::filter([&](auto p) { return !vis.test(p); })
		| std::views::transform(flood_fill));
}
