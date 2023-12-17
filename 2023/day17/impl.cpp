import aoc;
using pos2d = kg::pos2d<>;

constexpr auto offsets = std::to_array<pos2d>({{0, -1}, {0, +1}, {-1, 0}, {+1, 0}});
enum direction { up, down, left, right };

struct state {
	int cost;
	pos2d p;
	direction d;
	auto operator<=>(state const&) const = default;
};

auto to_char(direction d) {
	switch (d) {
	case up: return '^';
	case down: return 'v';
	case left: return '<';
	case right: return '>';
	default: throw;
	}
}

auto find_path(auto const& terrain, pos2d const start_pos, pos2d const end_pos, int mmin, int mmax) {
	constexpr size_t width = 141;
	constexpr size_t height = 141;

	// Setup visited array
	auto visited = kg::matrix_t<char, width, height>{};

	// Initialize priority queue
	std::priority_queue<state> queue;
	queue.push({-0, start_pos, right});
	queue.push({-0, start_pos, down});

	while (!queue.empty()) {
		auto const [neg_cost, p, dir] = queue.top();
		queue.pop();

		if (p == end_pos) {
			return -neg_cost;
		}

		// Mark as visited
		if (visited[p.y][p.x] & (1 << dir))
			continue;
		visited[p.y][p.x] |= (1 << dir);

		for (std::size_t i = 0; i < offsets.size(); i++) {
			direction const adj_dir = (direction)i;

			// Can't move backwards
			if (dir == adj_dir || offsets[dir] + offsets[adj_dir] == pos2d{0, 0})
				continue;

			int cost = -neg_cost;
			for (int step = 1; step <= mmax; step++) {
				// Check that a position is in bounds
				pos2d const adj_pos = p + offsets[i] * step;
				if (adj_pos.x < 0 || adj_pos.x >= terrain[0].size() || adj_pos.y < 0 || adj_pos.y >= terrain.size())
					break;


				// Add neighbour to the stack
				int const adj_heat = terrain[adj_pos.y][adj_pos.x] - '0';
				cost += adj_heat;
				if (step >= mmin)
					queue.push({-cost, adj_pos, adj_dir});
			}
		}
	}

	throw;
}

constexpr auto part1(auto const& input) {
	return find_path(input, {0, 0}, pos2d(input.size() - 1, input[0].size() - 1), 1, 3);
}

constexpr auto part2(auto const& input) {
	return find_path(input, {0, 0}, pos2d(input.size() - 1, input[0].size() - 1), 4, 10);
}
