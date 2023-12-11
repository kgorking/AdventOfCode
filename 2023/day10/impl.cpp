import aoc;

using namespace std::string_view_literals;
using pos2d = kg::pos2d<>;

enum dir { none, up, down, left, right };

pos2d find_start_position(auto const& input) {
	int x = 0, y = 0;
	for (std::string_view line : input) {
		x = line.find('S');
		if (x != line.npos)
			break;
		y++;
	}
	return {x, y};
}

pos2d advance_position(pos2d p, dir d) {
	constexpr pos2d offsets[] = {{0, 0}, {0, -1}, {0, +1}, {-1, 0}, {+1, 0}};
	return p + offsets[d];
}

dir dir_from_tile(auto const& input, pos2d p, dir prev_dir) {
	switch (input[p.y][p.x]) {
	case '|': return (prev_dir == down) ? down : up;
	case '-': return (prev_dir == right) ? right : left;
	case 'L': return (prev_dir == down) ? right : up;
	case 'J': return (prev_dir == down) ? left : up;
	case '7': return (prev_dir == up) ? left : down;
	case 'F': return (prev_dir == up) ? right : down;
	case 'S':
		if (p.y - 1 > 0 && "|7F"sv.contains(input[p.y - 1][p.x])) 
			return up;
		if (p.x + 1 < input.size() && "|JL"sv.contains(input[p.y + 1][p.x]))
			return down;
		if (p.x - 1 > 0 && "-FL"sv.contains(input[p.y][p.x - 1]))
			return left;
		if (p.x + 1 < input[0].size() && "-J7"sv.contains(input[p.y][p.x + 1]))
			return right;
	default:
		throw;
	}
}

auto solve(auto const& input) {
	int area = 0;
	int pos_count = 0;
	pos2d pos = find_start_position(input), last_pos = pos;
	dir next_dir = dir_from_tile(input, pos, none);

	do {
		pos = advance_position(pos, next_dir);
		next_dir = dir_from_tile(input, pos, next_dir);
		area += (last_pos.x + pos.x) * (last_pos.y - pos.y);
		last_pos = pos;
		pos_count += 1;
	} while ('S' != input[pos.y][pos.x]);

	area = std::abs(area) / 2;
	auto const max_dist = pos_count / 2;
	return std::make_pair(max_dist, area - max_dist + 1); // Pick's theorem, good shit
}
