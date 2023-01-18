import common;
//#include "../common/matrix.h"

using namespace kg;

consteval void test_matrix_mul() {
	matrix_t<int, 3, 2> const mA{1, 4, 3, 9, 8, 6};
	matrix_t<int, 2, 2> const mB{1, 6, 2, 9};
	auto const mC = mat_multiply(mA, mB);

	if (mC.size() != 3 && mC[0].size() != 2)
		throw "result matrix is wrong size";

	matrix_t<int, 3, 2> const expected{9, 42, 21, 99, 20, 102};
	if (mC != expected)
		throw "incorrect result";
}

consteval void test_matrix_add() {
	matrix_t<int, 2, 3> const mA{6,1,4,3,9,2};
	matrix_t<int, 2, 3> const mB{4,9,3,8,1,3};
	auto const mC = mat_add(mA, mB);

	matrix_t<int, 2, 3> const expected{10,10,7,11,10,5};
	if (mC != expected)
		throw "incorrect result";
}

consteval void test_matrix_scale() {
	matrix_t<int, 2, 3> const mA{6,1,4,3,9,2};
	auto const mC = mat_scale(mA, 2);

	matrix_t<int, 2, 3> const expected{12,2,8,6,18,4};
	if (mC != expected)
		throw "incorrect result";
}

consteval void test_matrix_power() {
	matrix_t<int, 2, 2> const mA{2, 5, 1, 4};
	auto const mC = mat_power(mA, 3);

	matrix_t<int, 2, 2> const expected{48, 165, 33, 114};
	if (mC != expected)
		throw "incorrect result";
}

consteval void test_matrix_power_zero() {
	matrix_t<int, 3, 3> const mA{2, 4, 4, 5, 1, 4};
	auto const mC = mat_power(mA, 0);

	matrix_t<int, 3, 3> const expected{1, 0, 0, 0, 1, 0, 0, 0, 1};
	if (mC != expected)
		throw "incorrect result";
}

consteval void test_matrix_linrec() {
	auto const mLR = make_linrec(1, 1);
	if (mLR.size() != 2 && mLR[0].size() != 2)
		throw "incorrect size";

	// test one step of fibonacci
	auto const v = vector_t<int, 2>{5, 8};
	auto const result = mat_multiply(mLR, v);
	auto const expected = vector_t<int, 2>{8, 13};
	if (result != expected)
		throw "incorrect result";

	// test 33rd fibonacci number
	auto const mLR32 = mat_power(mLR, 32); // 32 steps
	auto const initial = vector_t<int, 2>{0, 1}; // first 2 fib numbers
	auto const result_32_33 = mat_multiply(mLR32, initial); // 33rd step
	auto const expected_33 = 3524578;
	if (result_32_33[1][0] != expected_33)
		throw "incorrect result";
}

int main() {
	test_matrix_mul();
	test_matrix_add();
	test_matrix_scale();
	test_matrix_power();
	test_matrix_power_zero();
	test_matrix_linrec();
}
