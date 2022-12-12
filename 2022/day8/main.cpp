// https://adventofcode.com/2022/day/8
#include <array>
#include <bitset>
#include <iostream>
#include <string_view>
#include <assert.h>
#include "../../common/bits.h"

static constexpr auto input = std::to_array<std::string_view>({
#include "input.txt"
});


int main() {
	constexpr auto data = input;
	constexpr size_t N = data.size();

	// Part 1
	// Initialize the max-edges that sweep the forest
	std::array<char, N> maxt, maxb, maxr, maxl;
	data[0].copy(maxt.data(), N);
	data[N - 1].copy(maxb.data(), N);
	for (size_t i = 0; i < N; i++) {
		maxl[i] = data[i][0];
		maxr[i] = data[i][N - 1];
	}

	// Bits set where a visible tree is located
	std::array<std::bitset<N>, N> trees;

	// Sweep the forrest
	for (size_t y = 1; y < N-1; y++) {
		size_t const iy = N - 1 - y;
		for (size_t x = 1; x < N - 1; x++) {
			size_t const ix = N - 1 - x;

			// left
			char const left = data[y][x];
			bool visible = (left > maxl[y]);
			bool b = visible | trees[y].test(x);
			trees[y].set(x, b);
			maxl[y] = visible ? left : maxl[y];

			// right
			char const right = data[y][ix];
			visible = (right > maxr[y]);
			b = visible | trees[y].test(ix);
			trees[y].set(ix, b);
			maxr[y] = visible ? right : maxr[y];

			// top
			char const top = left; // data[y][x];
			visible = (top > maxt[x]);
			b = visible | trees[y].test(x);
			trees[y].set(x, b);
			maxt[x] = visible ? top : maxt[x];

			// bottom
			char const bottom = data[iy][x];
			visible = (bottom > maxb[x]);
			b = visible | trees[iy].test(x);
			trees[iy].set(x, b);
			maxb[x] = visible ? bottom : maxb[x];
		}
	}

	// Count the trees
	size_t count = 2 * (N + N - 2); // edge trees
	for (int y = 1; y < N - 1; y++) {
		count += trees[y].count();
	}

	std::cout << "Part 1: " << count << '\n';



	// Part 2
	int max_scenic_score = 0;
	for (int y = 1; y < N-1; y++) {
		size_t const iy = N - 1 - y;

		// Pick row
		auto const row = trees[y];

		// x-position of first tree in the row
		int x = kg::bit_distance_left(row, 0);

		while (x < N - 1) {
			size_t const ix = N - 1 - x;
			
			// the height of the current tree
			char const tree_height = data[y][x];

			// Find tree distances
			int tree_l = 1;
			while (tree_l < x && data[y][x - tree_l] < tree_height)
				tree_l += 1;

			int tree_r = 1;
			while (tree_r < ix && data[y][x + tree_r] < tree_height)
				tree_r += 1;

			int tree_u = 1;
			while (tree_u < y && data[y - tree_u][x] < tree_height)
				tree_u += 1;

			int tree_d = 1;
			while (tree_d < iy && data[y + tree_d][x] < tree_height)
				tree_d += 1;

			// Calc scenic score
			int const scenic_score = tree_l * tree_r * tree_d * tree_u;
			if (scenic_score > max_scenic_score)
				max_scenic_score = scenic_score;

			// Move to next tree
			x += kg::bit_distance_left(row, x);
		}
	}

	std::cout << "Part 2: " << max_scenic_score << '\n';
}
