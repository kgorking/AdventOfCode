export module aoc:matrix;
import std;

export namespace kg {

namespace mat_ops {
	constexpr auto mul = [](auto acc, auto a, auto b) -> decltype(acc + (a*b)) {
		//if (a == std::numeric_limits<decltype(a)>::max())
		//	return acc;
		//if (b == std::numeric_limits<decltype(b)>::max())
		//	return acc;
		return acc + (a * b);
		};
	constexpr auto min = [](auto acc, auto a, auto b) {
		if (a == std::numeric_limits<decltype(a)>::max())
			return acc;
		if (b == std::numeric_limits<decltype(b)>::max())
			return acc;
		return std::min({ acc, (a + b) }); };
}

template <typename T, int R, int C>
	requires(R > 0 && C > 0)
using matrix_t = std::array<std::array<T, C>, R>;

template <typename T, int R>
using vector_t = matrix_t<T, R, 1>;

template <typename T>
auto make_dmatrix(int R, int C = R) {
	auto mat = std::vector<std::vector<T>>(R);
	for (auto& vec : mat)
		vec.resize(C);
	return mat;
};

// Sets a row in a matrix
template <typename T, int N, int M = N>
constexpr void set_row(matrix_t<T, N, M>& m, int r, std::ranges::range auto& row) {
	std::ranges::copy(row, m[r].begin());
}

template <typename T, int N, int M = N>
auto make_from_rows(std::ranges::range auto const&... rows) {
	matrix_t<T, N, M> m;
	int i = 0;
	(set_row(m, i++, rows), ...);
	return m;
}

// Set a matrix to it's identity
template <typename T, int N>
constexpr void mat_identity(matrix_t<T, N, N>& ident) {
	for (int i = 0; i < N; i++) {
		ident[i].fill(0);
		ident[i][i] = 1;
	}
}

// Set a matrix to zero
template <typename T, int N>
constexpr void mat_zero(matrix_t<T, N, N>& ident) {
	for (int i = 0; i < N; i++) {
		ident[i].fill(0);
	}
}

// Returns an identity matrix
template <typename T, int N>
[[nodiscard]] constexpr matrix_t<T, N, N> mat_identity() {
	matrix_t<T, N, N> ident;
	mat_identity(ident);
	return ident;
}

// Sets a matrix to its maximum values
template <typename T, int N, int M = N>
constexpr void mat_max(matrix_t<T, N, M>& max) {
	for (int i = 0; i < N; i++)
		max[i].fill(std::numeric_limits<T>::max());
}

// Returns a matrix filled with maximum values
template <typename T, int N, int M = N>
constexpr matrix_t<T, N, M> mat_max() {
	matrix_t<T, N, M> max;
	for (int i = 0; i < N; i++)
		max[i].fill(std::numeric_limits<T>::max());
	return max;
}

// Multiplies two matrices A and B using a ternary operator
template <typename T, int N, int RA, int CB>
[[nodiscard]] constexpr auto mat_mul_op(matrix_t<T, RA, N> const& a, matrix_t<T, N, CB> const& b, auto&& op) {
	matrix_t<T, RA, CB> r {};
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < RA; i++) {
			for (int j = 0; j < CB; j++) {
				r[i][j] = op(r[i][j], a[i][k], b[k][j]);
			}
		}
	}
	return r;
}

// Multiplies two matrices A and B using a ternary operator
template <typename T, int N, int RA, int CB>
constexpr void mat_mul_op(std::in_place_t, matrix_t<T, RA, N>& a, matrix_t<T, N, CB> const& b, auto&& op) {
	auto r = std::make_unique<matrix_t<T, RA, CB>>();
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < RA; i++) {
			for (int j = 0; j < CB; j++) {
				(*r)[i][j] = op((*r)[i][j], a[i][k], b[k][j]);
			}
		}
	}
	std::ranges::copy(*r, a.begin());
}

// Multiplies two matrices A and B
template <typename T, int N, int RA, int CB>
[[nodiscard]] constexpr auto mat_multiply(matrix_t<T, RA, N> const& a, matrix_t<T, N, CB> const& b) {
	return mat_mul_op(a, b, mat_ops::mul);
}

// Add two matrices A and B
template <typename T, int R, int C>
constexpr auto mat_add(matrix_t<T, R, C> const& a, matrix_t<T, R, C> const& b) {
	matrix_t<T, R, C> r;

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			r[i][j] = a[i][j] + b[i][j];
		}
	}

	return r;
}

// Scales matrix A by s
template <typename T, int R, int C>
constexpr auto mat_scale(matrix_t<T, R, C> const& a, T s) {
	matrix_t<T, R, C> r;

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			r[i][j] = a[i][j] * s;
		}
	}

	return r;
}

// Raise matrix to the power of p
template <typename T, int R, int C, typename Op = decltype(mat_ops::mul)>
	requires(R == C) // only for square matrices
constexpr void mat_power(matrix_t<T, R, C>& a, int p, Op&& op = Op {}) {
	if (p < 0)
		throw std::invalid_argument("p can not be negative");

	if (p == 0) {
		mat_identity<T, R>(a);
	} else {
		auto b = new matrix_t<T, R, C>();
		std::ranges::copy(a, b->begin());

		// a^p = a * a^(p - 1).
		p = p - 1;
		while (p > 0) {
			// If n is odd, a = a * b.
			if (p & 1)
				mat_mul_op(std::in_place, a, *b, op);

			// b = b * b.
			mat_mul_op(std::in_place, *b, *b, op);

			// n = n / 2.
			p = p >> 1;
		}

		delete b;
		//return a;
	}
}

// Create a matrix used for linear recurrences
// https://en.wikipedia.org/wiki/Linear_recurrence_with_constant_coefficients
template <typename... Ts>
	requires(sizeof...(Ts) > 0)
constexpr auto make_linrec(Ts... coefficients) {
	using T = decltype((Ts {}, ...));
	constexpr auto N = sizeof...(Ts);

	matrix_t<T, N, N> m;

	for (int i = 0; i < N - 1; i++) {
		for (int j = 0; j < N; j++) {
			m[i][j] = (i + 1 == j);
		}
	}

	auto last_row = m[N - 1].data() + N;
	((*--last_row = coefficients), ...);

	return m;
}

} // namespace kg