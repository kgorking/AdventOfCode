// https://adventofcode.com/2022/day/8
#include <array>
#include <bitset>
#include <iostream>
#include <string_view>

static constexpr auto input = std::to_array<std::string_view>({
#include "input.txt"
});

// count leading zeroes in a bitset
template <int N>
constexpr int clz(std::bitset<N> x) noexcept {
	constexpr ptrdiff_t Bitsperword = CHAR_BIT * sizeof(x._Getword(0));
	constexpr ptrdiff_t Words = (N - 1) / Bitsperword;
	constexpr ptrdiff_t Remainder = (N % Bitsperword);

	int count = std::countl_zero(x._Getword(Words));
	if (count == Bitsperword) {
		count = Remainder;
		for (ptrdiff_t i = Words - 1; i >= 0; i--) {
			auto const c = std::countl_zero(x._Getword(i));
			if (c != Bitsperword)
				return count + c;
			else
				count += Bitsperword;
		}
		return count;
	} else {
		return count - (Bitsperword - Remainder);
	}
}

// count trailing zeroes in a bitset
template <int N>
constexpr int ctz(std::bitset<N> x) noexcept {
	constexpr ptrdiff_t Bitsperword = CHAR_BIT * sizeof(x._Getword(0));
	constexpr ptrdiff_t Words = (N - 1) / Bitsperword;

	int count = 0;
	for (ptrdiff_t i = 0; i < Words + 1; i++) {
		auto const c = std::countr_zero(x._Getword(i));
		count += c;

		if (c != Bitsperword)
			break;
	}

	return (N < count) ? N : count;
}

// Negates a std::bitset
//template <unsigned int N>
//constexpr std::bitset<N> negate(std::bitset<N> const& bs) {
//	static constexpr std::bitset<N> bs_one{1};
//	int const lz = ctz(bs);
//	return (~(bs >> lz) | bs_one) << lz;
//}


// Returns the distance of the first set bit to the left of 'pos'
template <ptrdiff_t N>
constexpr int bit_distance_left(std::bitset<N> x, int const pos) {
	if (pos == N - 1)
		return 1;

	auto const mask = ~std::bitset<N>{} << (pos+1);
	x = x & mask;
	return x.none() ? (N-1-pos) : ctz(x) - pos;
}

// Returns the distance of the first set bit to the right of 'pos'
template <ptrdiff_t N>
constexpr int bit_distance_right(std::bitset<N> x, int const pos) {
	if (pos == 0)
		return 1;

	auto const mask = ~(~std::bitset<N>{} << pos);
	x = x & mask;
	return x.none() ? pos : clz(x) - (N-1-pos);
}

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
		int x = bit_distance_left(row, 0);

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
			x += bit_distance_left(row, x);
		}
	}

	std::cout << "Part 2: " << max_scenic_score << '\n';
}
