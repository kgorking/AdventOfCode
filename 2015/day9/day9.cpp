#include <iostream>
#include "input.h"

int calc_distance(std::array<city, 8> const& cities) {

	int dist = 0;
	for (size_t i = 0; i < cities.size() - 1; i++) {
		city const c1 = cities[i];
		city const c2 = cities[i + 1];

		for (auto const& [a, b, d] : distances) {
			if ((c1 == a && c2 == b) || (c1 == b && c2 == a)) {
				dist += d;
				break;
			}
		}
	}

	return dist;
}

int main() {
	std::array<city, 8> cities{
		city::AlphaCentauri,
		city::Snowdin,
		city::Tambi,
		city::Faerun,
		city::Norrath,
		city::Straylight,
		city::Tristram,
		city::Arbre
	};

	int min_dist = std::numeric_limits<int>::max();
	int max_dist = std::numeric_limits<int>::min();
	do {
		int const dist = calc_distance(cities);
		min_dist = std::min(min_dist, dist);
		max_dist = std::max(max_dist, dist);
	} while (std::next_permutation(cities.begin(), cities.end()));

	std::cout << "Shotest distance: " << min_dist << '\n';
	std::cout << "Longest distance: " << max_dist << '\n';
}
