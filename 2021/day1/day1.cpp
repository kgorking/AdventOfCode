#include <array>
#include <iostream>

constexpr std::array<int, 2000> input{
#include "input.txt"
};

void part1() {
	int increases = 0;
	for (size_t i = 0; i < input.size() - 1; ++i) {
		increases += (input[i] < input[i + 1]);
	}
	std::cout << increases << " increases\n";
}

void part2() {
	// A+B+C < B+C+D == A < D

	int sliding_increases = 0;
	for (size_t i = 0; i < input.size() - 3; ++i) {
		sliding_increases += (input[i] < input[i + 3]);
	}

	std::cout << sliding_increases << " sliding increases\n";
}

int main() {
	part1();
	part2();
}
