import std;

constexpr std::size_t max_y_rock = 166 + 2;
constexpr std::size_t sand_spawn_pos = 500;

using row = std::bitset<2 * sand_spawn_pos>;
using volume = std::array<row, max_y_rock + 1>;


struct pos {
	int x, y;
};

const auto input = std::to_array<std::vector<pos>>({
#if 1
#include "input.txt"
#else
	{{498, 4}, {498, 6}, {496, 6}}, {
		{503, 4}, {502, 4}, {502, 9}, {
			494, 9
		}
	}
#endif
});

volume fill_cave_paths() {
	volume cave;

	for (std::size_t i = 0; i < input.size(); i++) {
		std::vector<pos> const& path = input[i];

		for (std::size_t p = 1; p < path.size(); p++) {
			pos const p1 = path[p - 1];
			pos const p2 = path[p];

			auto const [x_min, x_max] = std::minmax(p1.x, p2.x);
			for (std::size_t x = x_min; x <= x_max; x++) {
				cave[p1.y][x] = true;
			}

			auto const [y_min, y_max] = std::minmax(p1.y, p2.y);
			for (int y = y_min; y <= y_max; y++) {
				cave[y][x_min] = true;
			}
		}
	}

	return cave;
}

bool spawn_sand(volume const& cave, volume& sand) {
	row unit;
	unit.set(sand_spawn_pos, true);

	for (std::size_t y = 1; y < cave.size(); y++) {
		row const cave_and_sand = (cave[y] | sand[y]);
		row const available = cave_and_sand & unit;

		if (available == unit) {
			if ((cave_and_sand & (unit >> 1)) == 0) {
				unit >>= 1;
			} else if ((cave_and_sand & (unit << 1)) == 0) {
				unit <<= 1;
			} else {
				sand[y - 1] |= unit;
				return true;
			}
		}
	}

	return false;
}

int main() {
	volume cave = fill_cave_paths();
	volume sand;

	// Part 1
	int i = 0;
	while (spawn_sand(cave, sand))
		i += 1;
	std::cout << "Part 1: " << i << '\n';

	// Part 2
	cave[cave.size() - 1].flip();
	sand = {};
	while (!sand[0].test(sand_spawn_pos))
		spawn_sand(cave, sand);

	std::size_t sand_count = std::accumulate(sand.begin(), sand.end(), std::size_t{0}, [](std::size_t val, row const& bs){
		return val + bs.count();
	});
	std::cout << "Part 2: " << sand_count << '\n';

	for (std::size_t y = 0; y < cave.size(); y++) {
		for (std::size_t x = 0; x < cave[0].size(); x++) {
			if (y == 0 && x == sand_spawn_pos)
				std::cout << '+';
			else if (cave[y][x])
				std::cout << '#';
			else if (sand[y][x])
				std::cout << '.';
			else
				std::cout << ' ';
		}
		std::cout << '\n';
	}
}
