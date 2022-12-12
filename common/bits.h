#pragma once
#include <bit>
#include <bitset>

// Contains bit- and bitset related stuff

namespace kg {
// count leading zeroes in a bitset
template <int N>
constexpr int clz(std::bitset<N> x) noexcept {
	constexpr ptrdiff_t Bitsperword = CHAR_BIT * sizeof(x._Getword(0));
	constexpr ptrdiff_t Words = (N - 1) / Bitsperword;
	constexpr ptrdiff_t Remainder = (N % Bitsperword);

	int count = std::countl_zero(x._Getword(Words));
	if (count == Bitsperword) {
		count = Remainder;
		for (ptrdiff_t i = Words - 1; i >= 0; i--) {
			auto const c = std::countl_zero(x._Getword(i));
			if (c != Bitsperword)
				return count + c;
			else
				count += Bitsperword;
		}
		return count;
	} else {
		return count - (Bitsperword - Remainder);
	}
}

// count trailing zeroes in a bitset
template <int N>
constexpr int ctz(std::bitset<N> x) noexcept {
	constexpr ptrdiff_t Bitsperword = CHAR_BIT * sizeof(x._Getword(0));
	constexpr ptrdiff_t Words = (N - 1) / Bitsperword;

	int count = 0;
	for (ptrdiff_t i = 0; i < Words + 1; i++) {
		auto const c = std::countr_zero(x._Getword(i));
		count += c;

		if (c != Bitsperword)
			break;
	}

	return (N < count) ? N : count;
}

// Negates a std::bitset
template <int N>
constexpr std::bitset<N> negate(std::bitset<N> const& bs) {
	static constexpr std::bitset<N> bs_one{1};
	int const lz = ctz(bs);
	return (~(bs >> lz) | bs_one) << lz;
}

// Returns the distance of the first set bit to the left of 'pos'
template <int N>
constexpr int bit_distance_left(std::bitset<N> x, int const pos) {
	if (pos == N - 1)
		return 1;

	auto const mask = ~std::bitset<N>{} << (pos + 1);
	x = x & mask;
	return x.none() ? (N - 1 - pos) : ctz(x) - pos;
}

// Returns the distance of the first set bit to the right of 'pos'
template <int N>
constexpr int bit_distance_right(std::bitset<N> x, int const pos) {
	if (pos == 0)
		return 1;

	auto const mask = ~(~std::bitset<N>{} << pos);
	x = x & mask;
	return x.none() ? pos : clz(x) - (N - 1 - pos);
}
} // namespace kg