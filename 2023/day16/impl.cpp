import aoc;

using pos2d = kg::pos2d<>;
enum direction { none, up, down, left, right };
constexpr auto offsets = std::to_array<pos2d>({{0, 0}, {0, -1}, {0, +1}, {-1, 0}, {+1, 0}});

// Convert a tile and direction to a new direction         none,	up,					down,				left,				right
constexpr auto lookup = kg::matrix_t<char, 5, 5>{{/* '.' */ {none,	up,					down,				left,				right},
												  /* '/' */ {none,	right,				left,				down,				up},
												  /* '\' */ {none,	left,				right,				up,					down},
												  /* '|' */ {none,	up,					down,				up | down << 4,		up | down << 4},
												  /* '-' */ {none,	left | right << 4,	left | right << 4,	left,				right}}};

auto to_index = [](char c) {
	switch (c) {
	case '.': return 0;
	case '/': return 1;
	case '\\': return 2;
	case '|': return 3;
	case '-': return 4;
	}
};
auto to_bit = [](char d) { return 1 << (d - 1); };
auto advance_position = [](pos2d p, char d) { return p + offsets[d]; };

constexpr int count_energized(auto const& input, pos2d start, direction dir) {
	auto energized = kg::make_dmatrix<char>(input.size(), input[0].size());
	auto stack = std::stack<std::pair<pos2d, char>>{};

	// Push a position + direction on the stack if it's valid
	auto push_stack = [&](pos2d p, char dir) {
		if (p.x >= 0 && p.x < input[0].size() && p.y >= 0 && p.y < input.size() && !(energized[p.y][p.x] & to_bit(dir)))
			stack.push({p, dir});
	};

	push_stack(start, dir);

	while (!stack.empty()) {
		auto const [pos, dir] = stack.top();
		stack.pop();

		// Mark the direction as visited
		energized[pos.y][pos.x] |= to_bit(dir);

		char const index = to_index(input[pos.y][pos.x]);
		int const new_dir = lookup[index][dir];

		push_stack(advance_position(pos, new_dir & 0xF), new_dir & 0xF);
		if (new_dir >> 4)
			push_stack(advance_position(pos, new_dir >> 4), new_dir >> 4);
	}

	auto count = [](auto const& vec) { return std::ranges::count_if(vec, [](char v) { return v > 0; }); };
	return *std::ranges::fold_left_first(energized | std::views::transform(count), std::plus<>{});
}

constexpr auto part1(auto const& input) {
	return count_energized(input, {0,0}, right);
}

constexpr auto part2(auto const& input) {
	// Build a vector of starting positions
	std::vector<std::pair<pos2d, direction>> starts;
	starts.reserve(2 * (input.size() + input[0].size()));

	for (char x = 0; x < input[0].size(); x++) {
		starts.emplace_back(pos2d{x, 0}, down);
		starts.emplace_back(pos2d{x, (char)input.size()-1}, up);
	}
	for (char y = 0; y < input.size(); y++) {
		starts.emplace_back(pos2d{0, y}, right);
		starts.emplace_back(pos2d{(char)input[0].size() - 1, y}, left);
	}

	auto count = [&input](auto pair) { return count_energized(input, pair.first, pair.second); };
	auto max = [](int a, int b) { return (a > b) ? a : b; };
	return std::transform_reduce(/*std::execution::par_unseq,*/ starts.begin(), starts.end(), 0, max, count);
}
