import aoc;

enum color { red, green, blue };
constexpr int color_max[3] = {12, 13, 14};

struct grab {
	int count;
	color color;
};
struct set {
	int values[3] = {0, 0, 0};

	set(std::initializer_list<grab> const& i) {
		for (grab g : i)
			values[g.color] += g.count;
	}

	bool valid() const {
		return (values[red] <= color_max[red]) && (values[green] <= color_max[green]) && (values[blue] <= color_max[blue]);
	}
};

const auto input = std::to_array<std::vector<set>>({
#include "input.txt"
});

int part1() {
	int sum = 0;
	int game_idx = 1;
	for (std::vector<set> const& game : input) {
		bool ok = true;
		for (set const& s : game)
			ok = ok && s.valid();
		sum += ok * game_idx;
		game_idx += 1;
	}

	return sum;
}

int part2() {
	int sum = 0;
	for (std::vector<set> const& game_vec : input) {
		int max_values[3] = {1, 1, 1};
		for (set const& g : game_vec) {
			max_values[red] = std::max(g.values[red], max_values[red]);
			max_values[green] = std::max(g.values[green], max_values[green]);
			max_values[blue] = std::max(g.values[blue], max_values[blue]);
		}
		sum += (max_values[red] * max_values[green] * max_values[blue]);
	}

	return sum;
}

int main() {
	std::cout << "Part 1: " << part1() << '\n';
	std::cout << "Part 2: " << part2() << '\n';
}
