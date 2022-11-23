#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <ranges>

using binary = std::bitset<12>;

// 1000 x 12 bits
std::array<binary, 1000> input{{
#include "input.txt"
}};

binary find_gamma_rate() {
	// Counts the number of ones in each of the 12 columns of the input
	std::array<int, 12> result{};
	for (auto const& elem : input) {
		for (int j = 0; j < 12; ++j) {
			result[11 - j] += elem[j];
		}
	}

	// Transfrom results to whether there are more ones than zeroes
	for (int& val : result) {
		val = (val > (1000 - val));
	}

	// Return it as a bitset
	return binary(result.data(), 12, 0, 1);
}

binary find_epsilon_rate(binary const gamma_rate) {
	return (~gamma_rate); // flip the bits
}

// Partition the input into ranges based on bits at 'offset'
// returns {most, least}
auto partition_input(std::ranges::input_range auto& range, int const offset) {
	auto const zeroes = std::ranges::partition(range, [offset](binary const b) {
		return b[11 - offset];
	});
	auto const ones = std::ranges::subrange{range.begin(), zeroes.begin()};

	if (ones.size() >= zeroes.size())
		return std::pair{ones, zeroes};
	else
		return std::pair{zeroes, ones};
}

int find_life_support_rating() {
	// Get the CO2 and oxygen subranges
	auto [oxygen_subrange, CO2_subrange] = partition_input(input, 0);

	// Find the oxygen binary value
	int offset = 1;
	while (oxygen_subrange.size() > 1) {
		auto const [new_range, _] = partition_input(oxygen_subrange, offset);
		oxygen_subrange = new_range;
		offset += 1;
	}
	binary const oxy = oxygen_subrange.front();

	// Find the CO2 binary value
	offset = 1;
	while (CO2_subrange.size() > 1) {
		auto const [_, new_range] = partition_input(CO2_subrange, offset);
		CO2_subrange = new_range;
		offset += 1;
	}
	binary const CO2 = CO2_subrange.front();

	return oxy.to_ulong() * CO2.to_ulong();
}

void part1() {
	binary const gamma_rate = find_gamma_rate();
	binary const epsilon_rate = find_epsilon_rate(gamma_rate);

	std::cout << "Part 1: power consumption of the submarine is " << gamma_rate.to_ulong() * epsilon_rate.to_ulong() << '\n';
}

void part2() {
	int const life_support_rating = find_life_support_rating();

	std::cout << "Part 2: life support rating of the submarine is " << life_support_rating << '\n';
}

int main() {
	part1();
	part2();
}
