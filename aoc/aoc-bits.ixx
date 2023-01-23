export module aoc : bits;
import std;

// Contains bit- and bitset related stuff

export namespace kg {
// count leading zeroes in a bitset
template <int N>
constexpr int clz(std::bitset<N> x) noexcept {
	constexpr ptrdiff_t Bitsperword = 8 * sizeof(x._Getword(0));
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
	constexpr ptrdiff_t Bitsperword = 8 * sizeof(x._Getword(0));
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

// Subtracts 1 from a bitset
template <int N>
constexpr std::bitset<N> bit_minus_one(std::bitset<N> bs) {
	std::bitset<N> m = 1;

	// Flip all the set bits
	// until we find a 1
	while (!(bs & m).any()) {
		bs = bs ^ m;
		m <<= 1;
	}

	// flip the rightmost 1 bit
	bs = bs ^ m;
	return bs;
}

// Add two bitset
template <int N>
constexpr std::bitset<N> bit_add(std::bitset<N> l, std::bitset<N> r) {
	// https://www.geeksforgeeks.org/add-two-numbers-without-using-arithmetic-operators/?ref=lbp
	while (r.any()) {
		auto const carry = l & r;
		l = l ^ r;
		r = carry << 1;
	}
	return l;
}

// Subtracts two bitset
template <int N>
constexpr std::bitset<N> bit_subtract(std::bitset<N> l, std::bitset<N> r) {
	// https://www.geeksforgeeks.org/subtract-two-numbers-without-using-arithmetic-operators/?ref=lbp
	while (r.any()) {
		auto const borrow = (~l) & r;
		l = l ^ r;
		r = borrow << 1;
	}
	return l;
}

// Returns the position of the first set bit
template <int N>
constexpr int find_first_set(std::bitset<N> x) noexcept {
	if (x.none())
		return N;
	//return static_cast<int>((x ^ ~negate(x)).count()) - 1;
	return N - clz(x & negate(x)) - 1;
}

// Returns the distance from 'pos' to the first set bit to the left of 'pos'
template <int N>
constexpr int bit_distance_left(std::bitset<N> x, int const pos) {
	//assert(pos >= 0 && pos < N);
	if (pos == N - 1)
		return 1;

	auto const mask = ~std::bitset<N>{} << (pos + 1);
	x = x & mask;
	return x.none() ? (N - 1 - pos) : ctz(x) - pos;
}

// Returns the distance from 'pos' to the first set bit to the right of 'pos'
template <int N>
constexpr int bit_distance_right(std::bitset<N> x, int const pos) {
	//assert(pos >= 0 && pos < N);
	if (pos == 0)
		return 1;

	auto const mask = ~(~std::bitset<N>{} << pos);
	x = x & mask;
	return x.none() ? pos : clz(x) - (N - 1 - pos);
}

// Returns the position of the first set bit to the left of 'pos'
template <int N>
constexpr int bit_position_left(std::bitset<N> x, int const pos) {
	//assert(pos >= 0 && pos < N);
	if (pos >= N - 1)
		return N;

	auto const mask = ~std::bitset<N>{} << (pos + 1);
	x = x & mask;
	return x.none() ? N : ctz(x);
}

// Returns the position to the first set bit to the right of 'pos'
template <int N>
constexpr int bit_position_right(std::bitset<N> x, int const pos) {
	//assert(pos >= 0 && pos < N);
	if (pos == 0)
		return N;

	auto const mask = ~(~std::bitset<N>{} << pos);
	x = x & mask;
	return x.none() ? N : clz(x) - (N - 1);
}

// Find the nth set bit
template<int N>
constexpr int find_nth_set(std::bitset<N> const& bs, int n) {
	int i = find_first_set(bs);
	if (i == N)
		return N;

	int count = 1;
	while(count < n) {
		i = bit_position_left(bs, i);
		count += 1;
	}
	return i;
}

// Iterate the indices in a bitset
template<int N>
constexpr void bit_iterate_set(std::bitset<N> const& bs, auto fn) {
	for (int i = find_first_set(bs); i < N; i = bit_position_left(bs, i)) {
		fn(i);
	}
}

// Generate the subsets of the set bits in a bitset.
// O(2^n), where n is number of set bits
template<int N>
constexpr void bit_subsets(std::bitset<N> const& bs, auto fn) {
	for (auto subset = bs; subset.any(); subset = bit_minus_one(subset) & bs)
		fn(subset);
}

// Generate the X-bits subsets of the set bits in a bitset.
// Only calls 'fn' if the subset has X bits set.
template<int N>
constexpr void bit_subsets_n(std::bitset<N> const& bs, int x, auto fn) {
	for (auto subset = bs; subset.any(); subset = bit_minus_one(subset) & bs) {
		if (subset.count() == x)
			fn(subset);
	}
}

} // namespace kg