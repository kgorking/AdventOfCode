import aoc;

using i64 = std::int64_t;
using pos = kg::pos2d<i64>;
enum direction { R, D, L, U };
constexpr auto offsets = std::to_array<pos>({{+1, 0}, {0, +1}, {-1, 0}, {0, -1}});

struct dig {
	direction dir;
	char count;
	int color;
};

constexpr auto part1(auto const& input) { // array of 'dig'
	i64 area = 0;
	i64 edge_length = 0;
	pos prev{0, 0};
	for (auto d : input) {
		pos const next = prev + offsets[d.dir] * d.count;
		area += (prev.x + next.x) * (prev.y - next.y);
		edge_length += d.count;
		prev = next;
	}
	return 1 + (edge_length / 2) + (std::abs(area) / 2);
}

constexpr auto part2(auto const& input) {
	i64 area = 0;
	i64 edge_length = 0;
	pos prev{0, 0};
	for (auto d : input) {
		int const count = d.color >> 4;
		int const dir = d.color & 0xF;
		pos const next = prev + offsets[dir] * count;
		area += (prev.x + next.x) * (prev.y - next.y);
		edge_length += count;
		prev = next;
	}
	return 1 + (edge_length / 2) + (std::abs(area) / 2);
}
