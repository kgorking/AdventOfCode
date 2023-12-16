import aoc;
#include <cassert>

using pos2d = kg::pos2d<>;
enum direction { none, up, down, left, right };
constexpr auto offsets = std::to_array<pos2d>({{0, 0}, {0, -1}, {0, +1}, {-1, 0}, {+1, 0}});

// Convert a tile and direction to a new direction
constexpr auto lookup = kg::matrix_t<int, 5, 5>{{/* '.' */ {none, up, down, left, right},
												 /* '/' */ {none, right, left, down, up},
												 /* '\' */ {none, left, right, up, down},
												 /* '|' */ {none, up, down, up | down << 4, up | down << 4},
												 /* '-' */ {none, left | right << 4, left | right << 4, left, right}}};

auto to_index = [](char c) { return std::string_view{R"(./\|-)"}.find(c); };
auto to_bit = [](char d) { return 1 << (d - 1); };
auto advance_position = [](pos2d p, char d) { return p + offsets[d]; };

constexpr auto part1(auto const& input) {
	auto energized = kg::make_dmatrix<char>(input.size(), input[0].size());
	auto stack = std::stack<std::pair<pos2d, char>>{};

	// Push a position + direction on the stack if it's valid
	auto push_stack = [&](pos2d p, char dir) {
		if (p.x >= 0 && p.x < input[0].size() && p.y >= 0 && p.y < input.size() && !(energized[p.y][p.x] & to_bit(dir)))
			stack.push({p, dir});
	};

	push_stack({0, 0}, right);

	while (!stack.empty()) {
		auto const [pos, dir] = stack.top();
		stack.pop();

		// Mark the direction as visited
		energized[pos.y][pos.x] |= to_bit(dir);

		char const index = to_index(input[pos.y][pos.x]);
		int const new_dir = lookup[index][dir];
		push_stack(advance_position(pos, new_dir & 0xF), new_dir & 0xF);

		if (new_dir & 0xF0)
			push_stack(advance_position(pos, new_dir >> 4), new_dir >> 4);
	}

	int sum = 0;
	for (auto const& vec : energized) {
		sum += std::ranges::fold_left(vec, 0, [](auto acc, auto v) { return acc + (v > 0); });
	}
	return sum;
}

constexpr auto part2(auto const& input) {
	return 0;
}
