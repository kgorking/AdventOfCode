#pragma once

namespace kg {

template <typename T, int R, int C = R>
	requires(R > 0 && C > 0)
using matrix_t = std::array<std::array<T, C>, R>;

//template <typename T, int R>
//using vector_t = matrix_t<T, R, 1>;

// Multiplies two matrices A (and B
template <typename T, int N, int RA, int CB>
constexpr auto mat_multiply(matrix_t<T, RA, N> const& a, matrix_t<T, N, CB> const& b) {
	matrix_t<T, RA, CB> r;

	for (int i = 0; i < RA; i++) {
		for (int j = 0; j < CB; j++) {
			r[i][j] = 0;
			for (int k = 0; k < N; k++) {
				r[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	return r;
}

// Raise matrix to the power of p
template <typename T, int R, int C>
constexpr matrix_t<T, R, C> mat_power(matrix_t<T, R, C> a, int p) {
	auto b = a;

	// a^p = a * a^(p - 1).
	p = p - 1;
	while (p > 0) {
		// If n is odd, a = a * b.
		if (p & 1)
			a = mat_multiply(a, b);

		// b = b * b.
		b = mat_multiply(b, b);

		// n = n / 2.
		p = p >> 1;
	}

	return a;
}

}