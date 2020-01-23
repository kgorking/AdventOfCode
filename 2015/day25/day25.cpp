#include <iostream>

// Calculate the code number from the row and column coordinates
int calc_iterations(int column, int row) {
	return (row - 1) * (column - 1)
		+ column * (column + 1) / 2
		+ row * (row - 1) / 2;
}

uint64_t calc_code(int iterations) {
	uint64_t constexpr factor1 = 252533;
	uint64_t constexpr factor2 = 33554393;
	uint64_t constexpr start_code = 20151125;

	uint64_t result = start_code;
	for (int i = 2; i <= iterations; i++) {
		result *= factor1;
		result %= factor2;
	}
	return result;
}

int main() {
	int const iterations = calc_iterations(3075, 2981);
	std::cout << "Code number: " << iterations << '\n';
	std::cout << "Code: " << calc_code(iterations) << '\n';
}
