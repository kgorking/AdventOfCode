export module aoc.day10;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(36, 81);
export constexpr auto expected_input = std::make_pair(798, 1816);

void traverse_all_paths(auto const& terrain, kg::pos2di const start_pos, auto&& is_at_end, auto&& valid_neighbour) {
	const size_t height = terrain.size();
	const size_t width = terrain[0].size();

	// Initialize priority queue
	std::priority_queue<kg::pos2di> queue;
	queue.push(start_pos);

	while (!queue.empty()) {
		auto const p = queue.top();
		queue.pop();

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
				queue.push(adj_pos);
		}
	}
}

export auto part1(auto&& input) {
	std::set<kg::pos2di> ends;

	auto at_end = [&](auto p, char val) {
		bool const is_end = 9 == val;
		if (is_end)
			ends.insert(p);
		return is_end;
		};
	auto good_neighbour = [](char me, char n) { return 1 == (n - me); };

	int sum = 0;
	for (int j = 0; j < input.size(); j++) {
		for (int i = 0; i < input[j].size(); i++) {
			if (input[j][i] == 0) {
				traverse_all_paths(input, { i, j }, at_end, good_neighbour);
				sum += ends.size();
				ends.clear();
			}
		}
	}

	return sum;
}

export auto part2(auto&& input) {
	int num_paths = 0;

	auto at_end = [&](auto p, char val) {
		bool const is_end = 9 == val;
		num_paths += is_end;
		return is_end;
	};
	auto good_neighbour = [](char me, char n) { return 1 == (n - me); };

	int sum = 0;
	for (int j = 0; j < input.size(); j++) {
		for (int i = 0; i < input[j].size(); i++) {
			if (input[j][i] == 0) {
				traverse_all_paths(input, { i, j }, at_end, good_neighbour);
				sum += num_paths;
				num_paths = 0;
			}
		}
	}

	return sum;
}
