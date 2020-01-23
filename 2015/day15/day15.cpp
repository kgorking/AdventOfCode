#include <iostream>
#include <array>
#include <valarray>
#include <numeric>

using ingredient = std::valarray<int>;

int calc_ingredient_score(ingredient i) {
	ingredient const sliced = i[std::slice(0, 4, 1)]; // skip the calories in the product
	if (sliced.min() <= 0)
		return 0;
	return std::reduce(std::begin(sliced), std::end(sliced), 1, std::multiplies<>{});
}

int find_max_4way_score(ingredient const& a, ingredient const& b, ingredient const& c, ingredient const& d) {
	int max = 0;

	for (int x = 0; x <= 100; x++) {
		for (int y = 0; y <= 100 - x; y++) {
			for (int z = 0; z <= 100 - x - y; z++) {
				int const w = 100 - x - y - z;
				ingredient const combined = a*x + b*y + c*z + d*w;
				if (combined[4] == 500) // part 2
					max = std::max(max, calc_ingredient_score(combined));
			}
		}
	}

	return max;
}

int main() {
	std::array<ingredient, 4> const input{{
		{ 5, -1, 0, 0, 5},	/*Sprinkles*/
		{-1,  3, 0, 0, 1},	/*PeanutButter*/
		{ 0, -1, 4, 0, 6},	/*Frosting*/
		{-1,  0, 0, 2, 8}	/*Sugar*/
	}};

	int const max = find_max_4way_score(input[0], input[1], input[2], input[3]);
	std::cout << "total score: " << max << '\n';
}
