#pragma once

namespace kg {

template <typename T, int N>
	requires(N > 0)
using matrix_t = std::array<std::array<T, N>, N>;

// Multiplies two matrices A and B
template <typename T, int N>
	requires(N > 0)
matrix_t<T, N> mat_multiply(matrix_t<T, N> const& a, matrix_t<T, N> const& b) {
	matrix_t<T, N> r;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			r[i][j] = 0;
			for (int k = 0; k < N; k++) {
				r[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	return r;
}

// Raise matrix to the power of p
template <typename T, int N>
	requires(N > 0)
matrix_t<T, N> mat_power(matrix_t<T, N> a, int p) {
	// Identity Matrix.
	matrix_t<T, N> b;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			b[i][j] = a[i][j];
		}
	}

	// a = a * a ^ (p - 1).
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