import std;
#include "../../common/matrix.h"

using namespace kg;

consteval void test_matrix_mul() {
	matrix_t<int, 3, 2> const mA{1, 4, 3, 9, 8, 6};
	matrix_t<int, 2, 2> const mB{1, 6, 2, 9};
	auto const mC = mat_multiply(mA, mB);

	if (mC.size() != 3 && mC[0].size() != 2)
		throw "result matrix is wrong size";

	matrix_t<int, 3, 2> const expected{9, 42, 21, 99, 20, 102};
	if (!std::ranges::equal(mC, expected))
		throw "incorrect result";
}

void test_matrix_power() {
	matrix_t<int, 2, 2> const mA{2, 5, 1, 4};
	auto const mC = mat_power(mA, 3);

	matrix_t<int, 2, 2> const expected{48, 165, 33, 114};
	if (!std::ranges::equal(mC, expected))
		throw "incorrect result";
}

int main() {
	test_matrix_mul();
	test_matrix_power();
}
