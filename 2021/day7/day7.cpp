#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>

constexpr std::array<int, 1000> input{
#include "input.txt"
};

int calc_fuel_cost(int from, int to) {
	int const dist = std::abs(to - from);
	return dist;
}

int calc_fuel_cost_p2(int from, int to) {
	int const dist = std::abs(to - from);
	return (dist * (dist + 1)) / 2;
}

void part1() {
	auto copy = input;
	std::ranges::sort(copy);

	// Find the median
	int const median = copy[copy.size() / 2];

	int cost = 0;
	for (int const from : copy) {
		cost += calc_fuel_cost(from, median);
	}

	std::cout << "Part 1: fuel cost: " << cost << '\n';
}

void part2() {
	auto copy = input;

	// Find the averages
	double const average = std::accumulate(copy.begin(), copy.end(), 0.0) / copy.size();
	int const low = static_cast<int>(std::floor(average));
	int const high = static_cast<int>(std::ceil(average));

	int cost1 = 0;
	int cost2 = 0;
	for (int const from : copy) {
		cost1 += calc_fuel_cost_p2(from, low);
		cost2 += calc_fuel_cost_p2(from, high);
	}

	std::cout << "Part 2: fuel cost: " << std::min(cost1, cost2) << '\n';
}

int main() {
	part1(); // 331067
	part2(); // 92881128
}
