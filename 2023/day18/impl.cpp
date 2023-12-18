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

struct edge {
	pos a, b;
};

i64 area_incl_edges(std::vector<edge> const& edges) {
	i64 area = 0;
	i64 edge_length = 0;
	for (auto const [last_pos, pos] : edges) {
		area += (last_pos.x + pos.x) * (last_pos.y - pos.y);
		edge_length += last_pos.steps_to(pos);
	}
	return 1 + (edge_length / 2) + (std::abs(area) / 2);
}

constexpr auto part1(auto const& input) { // array of 'dig'
	// Create polygon
	std::vector<edge> poly;
	pos prev{0, 0};
	for (auto d : input) {
		pos const next = prev + offsets[d.dir] * d.count;
		poly.emplace_back(prev, next);
		prev = next;
	}

	return area_incl_edges(poly);
}

constexpr auto part2(auto const& input) {
	// Create polygon
	std::vector<edge> poly;
	pos prev{0, 0};
	for (auto d : input) {
		int const count = (d.color & 0xFFFFF0) >> 4;
		int const dir = d.color & 0x00000F;
		pos const next = prev + offsets[dir] * count;
		poly.emplace_back(prev, next);
		prev = next;
	}

	return area_incl_edges(poly);
}
