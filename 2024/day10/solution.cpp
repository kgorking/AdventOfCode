export module aoc.day10;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(36, 81);
export constexpr auto expected_input = std::make_pair(798, 1816);

constexpr auto directions = std::to_array<kg::pos2di>({ { 0, -1 }, { 0, +1 }, { -1, 0 }, { +1, 0 } });

bool good_neighbour(char me, char n) {
	return 1 == (n - me);
};

template<class T>
void traverse_all_paths(kg::grid<T> const& grid, kg::pos2di const start_pos, auto&& is_at_end, auto&& valid_neighbour) {
	std::vector<kg::pos2di> stack;
	stack.push_back(start_pos);

	while (!stack.empty()) {
		auto const p = stack.back();
		stack.pop_back();

		if (!is_at_end(p)) {
			auto neighbours = directions
				| std::views::transform([&](auto d) { return p + d; })
				| std::views::filter([&](auto n) { return 1 == grid[n] - grid[p]; });
			//stack.insert_range(stack.end(), neighbours); // too slow
			for (auto n : neighbours)
				stack.push_back(n);
		}
	}
}

export auto part1(auto&& input) {
	auto grid = kg::grid { input, -1 };
	auto visited = kg::visited<short>(grid);

	short curr_loop = 1; // use this so I don't have to clear the 'visited' matrix
	int num_paths = 0;

	auto at_end = [&](kg::pos2di p) {
		if (visited.test_or_set(p, curr_loop))
			return true;

		char const val = grid[p];
		num_paths += (9 == val);
		return (9 == val);
	};

	auto calc_path_scores = grid.values_and_coords()
		| kg::views::filter_eq(0, kg::select<0>)
		| std::views::values
		| std::views::transform([&](kg::pos2di p) {
				::traverse_all_paths(grid, p, at_end, good_neighbour);
				curr_loop += 1;
				return std::exchange(num_paths, 0);
			});

	return kg::sum(calc_path_scores);
}

export auto part2(auto&& input) {
	auto grid = kg::grid { input, -1 };

	auto traverse = [&](this auto const& self, kg::pos2di p) -> int {
		if (9 == grid[p])
			return 1;

		auto scores = directions
			| std::views::transform([&](auto d) { return p + d; })
			| std::views::filter([&](auto n) { return 1 == grid[n] - grid[p]; })
			| std::views::transform([&](auto n) { return self(n); });
		return kg::sum(scores);
	};

	auto calc_path_scores = grid.values_and_coords()
		| kg::views::filter_eq(0, kg::select<0>)
		| std::views::values
		| std::views::transform(traverse);

	return kg::sum(calc_path_scores);
}
