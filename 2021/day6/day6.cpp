#include <array>
#include <iostream>

constexpr std::array<uint8_t, 300> input{
#include "input.txt"
};

// Todo: rotate a histogram

template <size_t Days>
int64_t calc_spawn(int age, int days, std::array<int64_t, Days>& cache) {
	int cycle = days - age;
	int64_t total_spawn = 0;

	while (cycle > 0) {
		int64_t& cached_val = cache[cycle];
		if (cached_val == -1) {
			cached_val = calc_spawn(8, cycle - 1, cache);
		}

		total_spawn += 1 + cached_val;
		cycle -= 7;
	}

	return total_spawn;
}

template <size_t Days>
int64_t calc_lantern_fish() {
	std::array<int64_t, Days> cache;
	cache.fill(-1);

	int64_t total = 0;
	for (int const i : input) {
		total += calc_spawn(i, Days, cache);
	}

	return input.size() + total;
}

void part1() {
	std::cout << "Part 1: " << calc_lantern_fish<80>() << '\n';
}

void part2() {
	std::cout << "Part 2: " << calc_lantern_fish<256>() << '\n';
}

int main() {
	part1();
	part2();
}
