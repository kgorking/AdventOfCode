import aoc;

struct pair { int a, b; };
constexpr auto input = std::to_array<pair>({
	#include "input.txt"
});

constexpr char lookup[6][3] = {
// rock(1), paper(2), scissor(3)
	{4, 1, 7}, // vs rock
	{8, 5, 2}, // vs paper
	{3, 9, 6}, // vs scissor

	{3, 1, 2}, // lose(+0) - rotate right
	{4, 5, 6}, // draw(+3) - rotate none
	{8, 9, 7}  // win(+6)  - rotate left
};

int main() {
	pair res{0, 0};
	for(pair const p : input) {
		res.a += lookup[0 + p.b][p.a]; // Part 1
		res.b += lookup[3 + p.b][p.a]; // Part 2
	}

	std::cout
		<< res.a << '\n'
		<< res.b << '\n';
}
