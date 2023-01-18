import common;

struct pos {
	short x = 0, y = 0;

	pos operator + (pos p) const {
		return {x + p.x, y + p.y};
	}
};

#if 1
constexpr pos S{0,20};
constexpr pos E{158,20};
constexpr size_t height = 41;
constexpr size_t width = 181;
const auto input = std::to_array<std::string>({
#include "input.txt"
});
#else
constexpr pos S;	   //{0,20};
constexpr pos E{5, 2}; //{158,20};
constexpr size_t height = 5;
constexpr size_t width = 8;
const std::array<std::string, 5> input{{
		"Sabqponm",
		"abcryxxl",
		"accszExk",
		"acctuvwj",
		"abdefghi"}};
#endif

constexpr std::uint16_t max_distance = std::numeric_limits<std::uint16_t>::max();
constexpr auto offsets = std::to_array<pos>({{0, 1}, {0, -1}, {1, 0}, {-1, 0}});

constexpr bool in_bounds(pos p) {
	return (p.x >= 0 && p.x <= width - 1 && p.y >= 0 && p.y <= height - 1);
}

auto find_path(pos S, pos E, auto terrain, auto comp) {
	// Setup visited array
	std::array<std::bitset<width>, height> visited;
	visited.fill({0});

	// Setup distance array
	std::array<std::array<std::uint16_t, width>, height> edge_distances;
	for (auto& edges : edge_distances)
		edges.fill(max_distance);

	auto const queue_compare = [&](pos l, pos r) {
		return edge_distances[l.y][l.x] > edge_distances[r.y][r.x];
	};
	std::priority_queue<pos, std::vector<pos>, decltype(queue_compare)> queue(queue_compare);
	queue.push(S);
	edge_distances[S.y][S.x] = 0;

	while (!queue.empty()) {
		pos const p = queue.top();
		queue.pop();

		// Mark as visited
		if (visited[p.y][p.x])
			continue;
		visited[p.y][p.x] = true;

		char const elevation = terrain[p.y][p.x];
		std::uint16_t const distance = edge_distances[p.y][p.x];

		for (std::size_t i = 0; i < offsets.size(); i++) {
			pos const adj_pos = p + offsets[i];

			if (!in_bounds(adj_pos))
				continue;

			// Find distance to adjacent cell
			char const adj_elev = terrain[adj_pos.y][adj_pos.x];
			int const elev_diff = adj_elev - elevation;

			if (comp(elev_diff)) { //  <= 1
				if (edge_distances[adj_pos.y][adj_pos.x] > distance + 1) {
					edge_distances[adj_pos.y][adj_pos.x] = distance + 1;
					queue.push(adj_pos);
				}
			}
		}
	}

	return edge_distances;
}

int main() {
	std::cout << std::format("width: {} {}\n", width, input[0].size());
	std::cout << std::format("height: {} {}\n", height, input.size());
	std::cout << std::format("start: {}, end: {}\n\n", input[S.y][S.x], input[E.y][E.x]);

	auto terrain = input;
	terrain[S.y][S.x] = 'a';
	terrain[E.y][E.x] = 'z';

	// Part 1
	auto const edge_distances = find_path(S, E, terrain, [](int c) {
		return c <= 1;
	});
	uint16_t const distance_to_E = edge_distances[E.y][E.x];
	std::cout << "Part 1: " << distance_to_E << '\n';


	// Part 2
	auto const rev_edge_distances = find_path(E, S, terrain, [](int c) {
		return c >= -1;
	});
	int distance_to_a = 9999;
	for (std::size_t y = 0; y < height; y++) {
		for (std::size_t x = 0; x < width; x++) {
			if (terrain[y][x] != 'a')
				continue;

			int const dist = rev_edge_distances[y][x];
			if (dist < distance_to_a)
				distance_to_a = dist;
		}
	}

	std::cout << "Part 2: " << distance_to_a;
}
