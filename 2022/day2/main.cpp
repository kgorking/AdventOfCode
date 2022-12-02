#include <array>
#include <iostream>

struct pair { char a, b; };
constexpr auto input = std::to_array<pair>({
	#include "input.txt"
});

constexpr char matrix_p1[3][3] = {
// rock(1), paper(2), scissor(3)
	{4, 1, 7}, // rock
	{8, 5, 2}, // paper
	{3, 9, 6}  // scissor
};

constexpr char matrix_p2[3][3] = {
// rock(1), paper(2), scissor(3)
	{3, 1, 2}, // lose(+0) - rotate right
	{4, 5, 6}, // draw(+3) - rotate none
	{8, 9, 7}  // win(+6)  - rotate left
};

int main() {
	int res1 = 0;
	int res2 = 0;
	for(pair const p : input) {
		res1 += matrix_p1[p.b][p.a];
		res2 += matrix_p2[p.b][p.a];
	}

	// Part 1
	std::cout << res1 << '\n';

	// Part 2 <14058
	std::cout << res2 << '\n';
}
