// https://adventofcode.com/2022/day/9
import common;

enum Dir : char { L, R, U, D };
struct move {
	Dir dir;
	char distance;
};

static constexpr auto input = std::to_array<move>({
#if 1
#include "input.txt"
#else
	{R, 5}, {U, 8}, {L, 8}, {D, 3}, {R, 17}, {D, 10}, {L, 25}, {
		U, 20
	}
#endif
});

using pos = kg::pos2d<int>;

// L R U D
static constexpr pos offsets[] = {pos{-1, 0}, pos{1, 0}, pos{0, 1}, pos{0, -1}};

template <int Knots>
size_t simulate() {
	std::array<pos, Knots> knots;
	std::set<pos> visited;

	visited.insert(knots[0]);
	for (move m : input) {
		for (int i = 0; i < m.distance; i++) {
			knots[0].x += offsets[m.dir].x;
			knots[0].y += offsets[m.dir].y;

			bool modified = false;
			for (int k = 1; k < Knots; k++) {
				modified = knots[k].follow(knots[k - 1]);
			}

			if (modified)
				visited.insert(knots[Knots - 1]);
		}
	}

	return visited.size();
}

int main() {
	std::cout << "Part 1: " << simulate<2>() << '\n';
	std::cout << "Part 2: " << simulate<10>() << '\n';
}
