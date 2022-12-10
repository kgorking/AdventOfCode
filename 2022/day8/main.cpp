// https://adventofcode.com/2022/day/8
#include <array>
#include <bitset>
#include <iostream>
#include <string_view>
#include <format>
#include <assert.h>

static constexpr auto input = std::to_array<std::string_view>({
#include "input.txt"
});

static constexpr auto test = std::to_array<std::string_view>({
	"30373",
	"25512",
	"65332",
	"33549",
	"35390"
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

	return std::min(N, count);
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

	/*auto const bs = std::bitset<10>(0b0010001000);
	std::cout << bs << " i    L R\n-------------------\n";
	for (int i = 0; i < 10; i++) {
		auto str = bs.to_string();
		str[9-i] = '-';
		int const left = bit_distance_left(bs, i);
		int const right = bit_distance_right(bs, i);
		std::cout << std::format("{}[{}] - {} {}\n", str, i, left, right);
	}
	return 0;*/

	// Part 1

	// Initialize the max-edges that sweep the forest
	std::array<char, N> maxt, maxb, maxr, maxl;
	data[0].copy(maxt.data(), N);
	data[N - 1].copy(maxb.data(), N);
	for (size_t i = 0; i < N; i++) {
		maxl[i] = data[i][0];
		maxr[i] = data[i][N - 1];
	}

	std::array<std::bitset<N>, N> trees;
	std::array<std::bitset<N>, N> treesT; // transpose of 'trees'

	std::array<std::bitset<N>, N> lefts;
	std::array<std::bitset<N>, N> right;
	std::array<std::bitset<N>, N> ups;
	std::array<std::bitset<N>, N> downs;

	for (size_t y = 0; y < N; y++) {
		for (size_t x = 0; x < N; x++) {
			size_t const iy = N - 1 - y;
			size_t const ix = N - 1 - x;

			// left
			char const left = data[y][x];
			bool visible = (left > maxl[y]);
			bool b = visible || trees[y].test(x);
			trees[y].set(x, b);
			treesT[x].set(y, b);
			maxl[y] = visible ? left : maxl[y];

			// right
			char const right = data[y][ix];
			visible = (right > maxr[y]);
			b = visible || trees[y].test(ix);
			trees[y].set(ix, b);
			treesT[ix].set(y, b);
			maxr[y] = visible ? right : maxr[y];

			// top
			char const top = left; // data[y][x];
			visible = (top > maxt[x]);
			b = visible || trees[y].test(x);
			trees[y].set(x, b);
			treesT[x].set(y, b);
			maxt[x] = visible ? top : maxt[x];

			// bottom
			char const bottom = data[iy][x];
			visible = (bottom > maxb[x]);
			b = visible || trees[iy].test(x);
			trees[iy].set(x, b);
			treesT[x].set(iy, b);
			maxb[x] = visible ? bottom : maxb[x];
		}
	}

	auto count = 2 * (data.size() + data[0].size() - 2);
	auto const mask = ~((std::bitset<N>{1} << N - 1) | std::bitset<N>{1});
	for (int y = 1; y < N - 1; y++) {
		auto const row = trees[y] & mask;
		count += row.count();

		std::cout << row.to_string() << '\n';
	}
	std::cout << "Part 1: " << count << '\n';



	// Part 2
	int max_scenic_score = 0;
	for (int y = 1; y < N-1; y++) {
		// Pick row
		auto const row = trees[y];

		// x-position of first tree in the row
		int x = bit_distance_left(row, 0);

		//std::cout << "y=" << y << ": ";
		while (x < N - 1) {
			assert(row.test(x));
			auto const column = treesT[x];

			// the height of the current tree
			char const tree_height = data[y][x];

			// Find tree distances
			int tree_l = bit_distance_left(row, x);
			//while (x + tree_l < N-1 && tree_height > data[y][x + tree_l])
			//	tree_l += bit_distance_left(row, x + tree_l);

			int tree_r = bit_distance_right(row, x);
			//while (x - tree_r > 0 && tree_height > data[y][x - tree_r])
			//	tree_r += bit_distance_right(row, x - tree_r);

			int tree_d = bit_distance_left(column, y);
			//while (y + tree_d < N - 1 && tree_height > data[y + tree_d][x])
			//	tree_d += bit_distance_left(column, y + tree_d);

			int const tree_u = bit_distance_right(column, y);

			//int const dist_l = (tree_height > data[y][x + tree_l]) ? N - 1 - x : tree_l;
			//int const dist_r = (tree_height > data[y][x - tree_r]) ? x : tree_r;
			//int const dist_d = (tree_height > data[y + tree_d][x]) ? N - 1 - y : tree_d;
			//int const dist_u = (tree_height > data[y - tree_u][x]) ? y : tree_u;

			// Calc scenic score
			int const scenic_score = tree_l * tree_r * tree_d * tree_u;
			if (scenic_score > max_scenic_score)
				max_scenic_score = scenic_score;

			// Move to next tree
			x += tree_l;

			//std::cout << std::format("{}*{}*{}*{}  ", dist_u, dist_r, dist_l, dist_d);
		}
		//std::cout << '\n';
	}

	// >79200
	// >200640
	// !237699
	// !287640
	// <352000
	std::cout << "Part 2: " << max_scenic_score << '\n';
}
