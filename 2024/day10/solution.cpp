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

void traverse_all_paths(auto const& terrain, kg::pos2di const start_pos, auto&& is_at_end, auto&& valid_neighbour) {
	std::vector<kg::pos2di> queue;
	queue.push_back(start_pos);

	while (!queue.empty()) {
		auto const p = queue.back();
		queue.pop_back();

		if (!is_at_end(p)) {
			auto neighbours = directions
				| std::views::transform([&](auto d) { return p + d; })
				| std::views::filter([&](auto n) { return kg::is_valid(terrain, n) && 1 == terrain[n.y][n.x] - terrain[p.y][p.x]; });
			//queue.insert_range(queue.end(), neighbours); // too slow
			for (auto n : neighbours)
				queue.push_back(n);
		}
	}
}

export auto part1(auto&& input) {
	auto visited = kg::make_dmatrix<short>(input.size(), input.size());

	int curr_loop = 1; // use this so I don't have to clear the 'visited' matrix
	int num_paths = 0;

	auto at_end = [&](kg::pos2di p) {
		if (visited[p.y][p.x] == curr_loop)
			return true;
		visited[p.y][p.x] = curr_loop;

		char const val = kg::at(input, p, char(0));
		num_paths += (9 == val);
		return (9 == val);
	};

	auto calc_path_scores = input
		| kg::views::with_coord2d // -> {key: char, value: kg::pos2di}
		| kg::views::filter_eq(0, kg::select<0>)
		| std::views::values
		| std::views::transform([&](kg::pos2di p) {
				::traverse_all_paths(input, p, at_end, good_neighbour);
				curr_loop += 1;
				return std::exchange(num_paths, 0);
			});

	return kg::sum(calc_path_scores);
}

export auto part2(auto&& input) {
	auto traverse = [&](this auto const& self, kg::pos2di p) -> int {
		if (9 == input[p.y][p.x])
			return 1;

		auto scores = directions
			| std::views::transform([&](auto d) { return p + d; })
			| std::views::filter([&](auto n) { return kg::is_valid(input, n) && 1 == input[n.y][n.x] - input[p.y][p.x]; })
			| std::views::transform([&](auto n) { return self(n); });
		return kg::sum(scores);
	};

	auto calc_path_scores = input
		| kg::views::with_coord2d // -> {key: char, value: kg::pos2di}
		| kg::views::filter_eq(0, kg::select<0>)
		| std::views::values
		| std::views::transform(traverse);

	return kg::sum(calc_path_scores);
}
