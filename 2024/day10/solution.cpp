export module aoc.day10;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(36, 81);
export constexpr auto expected_input = std::make_pair(798, 1816);

// also exists in 'aoc-graph.ixx'
void traverse_all_paths(auto const& terrain, kg::pos2di const start_pos, auto&& is_at_end, auto&& valid_neighbour) {
	const size_t height = terrain.size();
	const size_t width = terrain[0].size();

	// Initialize queue
	std::vector<kg::pos2di> queue;
	queue.push_back(start_pos);

	while (!queue.empty()) {
		auto const p = queue.back();
		queue.pop_back();

		if (is_at_end(p, terrain[p.y][p.x])) {
			continue;
		}

		for (auto const dir : std::to_array<kg::pos2di>({ { 0, -1 }, { 0, +1 }, { -1, 0 }, { +1, 0 } })) {
			// Check that the position is in bounds
			kg::pos2di const adj_pos = p + dir;
			if (adj_pos.x < 0 || adj_pos.x >= width || adj_pos.y < 0 || adj_pos.y >= height)
				continue;

			// Add neighbour to the stack
			int const adj_val = terrain[adj_pos.y][adj_pos.x];
			if (valid_neighbour(terrain[p.y][p.x], adj_val))
				queue.push_back(adj_pos);
		}
	}
}

bool good_neighbour(char me, char n) { return 1 == (n - me); };

export auto part1(auto&& input) {
	std::set<kg::pos2di> ends;

	auto at_end = [&](kg::pos2di p, char val) {
		bool const is_end = 9 == val;
		if (is_end)
			ends.insert(p);
		return is_end;
		};

	auto enum_grid_and_calc_path_scores
		= input
		| kg::views::coord2d
		| kg::views::filter_eq(0, kg::select<0>)
		| std::views::values
		| std::views::transform([&](kg::pos2di p) {
			traverse_all_paths(input, p, at_end, good_neighbour);
			int const sum = ends.size();
			ends.clear();
			return sum;
		});

	return kg::sum(enum_grid_and_calc_path_scores);
}

export auto part2(auto&& input) {
	int num_paths = 0;

	auto at_end = [&](kg::pos2di _, char val) {
		bool const is_end = 9 == val;
		num_paths += is_end;
		return is_end;
		};

	auto enum_grid_and_calc_path_scores
		= input
		| kg::views::coord2d
		| kg::views::filter_eq(0, kg::select<0>)
		| std::views::values
		| std::views::transform([&](kg::pos2di p) {
			traverse_all_paths(input, p, at_end, good_neighbour);
			return std::exchange(num_paths, 0);
		});

	return kg::sum(enum_grid_and_calc_path_scores);
}
