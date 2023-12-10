import aoc;

auto input = std::to_array<std::string_view>({
#include "input.txt"
});

using namespace std::string_view_literals;
using pos2d = kg::pos2d<>;

enum dir { none, up, down, left, right };

int follow_pipe(int x, int y) {
	return 0;
}

pos2d find_start_position() {
	int x = 0, y = 0;
	for (std::string_view line : input) {
		x = line.find('S');
		if (x != line.npos)
			break;
		y++;
	}
	return {x, y};
}

bool is_start_position(pos2d p) {
	return 'S' == input[p.y][p.x];
}

pos2d advance_position(pos2d p, dir d) {
	pos2d const offsets[] = {{0, 0}, {0, -1}, {0, +1}, {-1, 0}, {+1, 0}};
	return p + offsets[d];
}

dir dir_from_tile(pos2d p, dir prev_dir) {
	char const tile = input[p.y][p.x];
	switch (tile) {
	case '|':
		return (prev_dir == down) ? down : up;
	case '-':
		return (prev_dir == right) ? right : left;
	case 'L':
		return (prev_dir == down) ? right : up;
	case 'J':
		return (prev_dir == down) ? left : up;
	case '7':
		return (prev_dir == up) ? left : down;
	case 'F':
		return (prev_dir == up) ? right : down;
	case 'S':
		if (p.y - 1 > 0 && "|7F"sv.contains(input[p.y - 1][p.x]) && prev_dir != up)
			return up;
		if (p.x + 1 < input.size() && "|JL"sv.contains(input[p.y + 1][p.x]) && prev_dir != down)
			return down;
		if (p.x - 1 > 0 && "-FL"sv.contains(input[p.y][p.x - 1]) && prev_dir != left)
			return left;
		if (p.x + 1 < input[0].size() && "-J7"sv.contains(input[p.y][p.x + 1]) && prev_dir != right)
			return right;
	default:
		throw;
	}
}

int test1() {
	pos2d pos = find_start_position();
	dir next_dir = dir_from_tile(pos, none);

	int counter = 0;

	do {
		counter++;
		pos = advance_position(pos, next_dir);
		next_dir = dir_from_tile(pos, next_dir);
	} while (!is_start_position(pos));

	return counter / 2;
}

int test2() {
	pos2d pos = find_start_position();
	dir next_dir = dir_from_tile(pos, none);

	int counter = 0;

	do {
		//input[pos.y][pos.x] = counter++;
		pos = advance_position(pos, next_dir);
		next_dir = dir_from_tile(pos, next_dir);
	} while (!is_start_position(pos));

	return counter / 2;
}

int part1() {
	return 0;
}
int part2() {
	return 0;
}

int main() {
	 std::cout << "Test 1: " << test1() << '\n';
	 std::cout << "Test 2: " << test2() << '\n';
	// std::cout << "Part 1: " << 0 << '\n';
	// std::cout << "Part 2: " << 0 << '\n';
}
