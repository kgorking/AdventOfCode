export module aoc.day16;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(7036, 45);
export constexpr auto expected_input = std::make_pair(88468, 0);


export auto part1(auto&& input) {
	using node = std::pair<int, kg::cursor>;
	using cmp = decltype([](node const& a, node const& b) { return a.first < b.first; });

	kg::pos2di const S { 1, (int)std::ssize(input) - 2 };
	kg::pos2di const E { (int)std::ssize(input) - 2, 1 };

	int min_cost = std::numeric_limits<int>::max();
	kg::grid map { input };
	kg::visited<int> vis ( map, min_cost );

	std::priority_queue<node, std::vector<node>, cmp> queue;
	queue.push({ 0, { S, kg::direction::right() } });

	while(!queue.empty()) {
		auto const [cost, cursor] = queue.top();
		queue.pop();

		if (cost > vis[cursor])
			continue;
		vis.set(cursor, cost);

		if (cursor.pos == E) {
			min_cost = std::min({ min_cost, cost });
			continue;
		}

		for(kg::cursor n : cursor.neighbours_not_behind()) {
			if (map[n] == '#')
				continue;

			int const this_cost = (n.dir == cursor.dir) ? 1 : 1001;
			int const new_cost = cost + this_cost;
			queue.push({ new_cost, n });
		}
	}

	return min_cost;
}

export auto part2(auto&& input) {
	// The node used in the priority queue
	// Holds cost, position+dir
	using node = std::tuple<int, kg::cursor>;
	using cmp = decltype([](node const& a, node const& b) { return std::get<0>(a) < std::get<0>(b); });

	// The start- and end position
	kg::pos2di const S { 1, (int)std::ssize(input) - 2 };
	kg::pos2di const E { (int)std::ssize(input) - 2, 1 };

	kg::grid map { input };

	int min_cost = std::numeric_limits<int>::max();
	kg::visited<int> costs(map, min_cost);
	kg::visited dirs(map); // todo!

	// Load the starting position and direction onto the queue
	std::priority_queue<node, std::vector<node>, cmp> queue;
	queue.push({ 0, { S, kg::direction::right() }});

	auto draw_map = [&input](auto& map, kg::pos2di star = {-1,-1}) {
		for (auto [j, sv] : input | std::views::enumerate) {
			for (auto [i, c] : sv | std::views::enumerate) {
				if (star == kg::pos2di{ (int)i, (int)j }){
					std::putchar('*');
				} else if (map.contains({ (int)i, (int)j }))
					std::putchar('.');
				else {
					std::putchar(c == '.' ? ' ' : c);
				}
			}
			std::println();
		}
		std::println();
	};

	while (!queue.empty()) {
		auto [cost, cursor] = queue.top();
		queue.pop();

		// If this node's cost is greater than a previous node,
		// it is discarded.
		if (cost > costs[cursor]) {
			//std::println("cost: {} vs {}", cost, costs[cursor]);
			//draw_map(path, cursor.pos);
			break;
		}

		// Update the lowest cost at this position
		costs.set(cursor, cost);

		if (E == cursor.pos) {
			// I have reached the end
			if(cost < min_cost) {
				// I have new path with the lowest cost, so
				// update the minimum
				min_cost = cost;
			}

			continue;
		}

		// Proceed to tiles ahead and to the sides
		for (kg::cursor n : cursor.neighbours_not_behind()) {
			// Ignore walls
			if (map[n] == '#')
				continue;

			// Cost is +1 to move forward, +1000 to turn
			int const this_cost = (n.dir == cursor.dir) ? 1 : 1001;

			// Add the neighbour to the queue
			queue.push({ cost + this_cost, n});
		}
	}

	// Part 2
	std::set<kg::pos2di> best_path_tiles;
	kg::visited pvis { map };
	auto find_best_paths = [&](this auto& self, kg::pos2di p) -> int {
		if (pvis.test(p))
			return 0;
		pvis.set(p);

		best_path_tiles.insert(p);

		int const cost = costs[p];

		int min_neighbour_cost = 100'000;
		for(int i=0; i<4; i++) {
			min_neighbour_cost = std::min(min_neighbour_cost, costs[p + kg::direction(i)]);
		}

		int count = 1;
		for (int i = 0; i < 4; i++) {
			auto const n = p + kg::direction(i);
			if (pvis.test(n))
				continue;

			int const n_cost = costs[n];
			if (n_cost == std::numeric_limits<int>::max())
				continue;

			if (n_cost > cost) {
				std::println("ignoring point {},{} with cost {} vs {}", n.x, n.y, n_cost, cost);
				draw_map(best_path_tiles, n);
				continue;
			}
			if(n_cost == min_neighbour_cost) {
				std::println("including point {},{} with cost {} vs {}", n.x, n.y, n_cost, cost);
				count += self(n);
			}
		}
		return count;
	};
	find_best_paths(E);
	draw_map(best_path_tiles);

	return (int)best_path_tiles.size();
}
