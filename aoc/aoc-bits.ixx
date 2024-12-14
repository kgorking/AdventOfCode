export module aoc:bits;
import std;
import :math;
import :async;

// Contains bit- and bitset related stuff

export namespace kg {

// TODO
template <int N>
std::bitset<N> to_bitset(std::string_view sv, char one) {
	std::bitset<N> bs;
	for (int i = 0; char c : sv)
		bs[i++] = (c == one) ? 1 : 0;
	return bs;
}

//

// count leading zeroes in a bitset
template <int N>
constexpr int clz(std::bitset<N> const& x) noexcept {
	constexpr ptrdiff_t Bitsperword = 64;
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
constexpr int ctz(std::bitset<N> const& x) noexcept {
	constexpr ptrdiff_t Bitsperword = 64;
	constexpr ptrdiff_t Words = (N - 1) / Bitsperword;

	int count = 0;
	auto masked = reinterpret_cast<std::uint64_t const*>(&x);
	for (ptrdiff_t i = 0; i < Words + 1; i++) {
		//auto masked = x & mask; // slow as fuck
		//x >>= 64;
		auto const c = std::countr_zero(masked[i]);
		count += c;

		if (c != Bitsperword)
			break;
	}

	return (N < count) ? N : count;
}

// Negates a std::bitset
template <int N>
constexpr std::bitset<N> negate(std::bitset<N> const& bs) {
	constexpr std::bitset<N> bs_one { 1 };
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

// Add two bitsets
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

// Subtracts two bitsets
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
constexpr int find_first_set(std::bitset<N> const& x) noexcept {
	return ctz(x);
}

// Returns the distance from 'pos' to the first set bit to the left of 'pos'
template <int N>
constexpr int bit_distance_left(std::bitset<N> x, int const pos) {
	// assert(pos >= 0 && pos < N);
	if (pos == N - 1)
		return 1;

	auto const mask = ~std::bitset<N> {} << (pos + 1);
	x = x & mask;
	return x.none() ? (N - 1 - pos) : ctz(x) - pos;
}

// Returns the distance from 'pos' to the first set bit to the right of 'pos'
template <int N>
constexpr int bit_distance_right(std::bitset<N> x, int const pos) {
	// assert(pos >= 0 && pos < N);
	if (pos == 0)
		return 1;

	auto const mask = ~(~std::bitset<N> {} << pos);
	x = x & mask;
	return x.none() ? pos : clz(x) - (N - 1 - pos);
}

// Returns the position of the first set bit to the left of 'pos'
template <int N>
constexpr int bit_position_left(std::bitset<N> x, int const pos) {
	// assert(pos >= 0 && pos < N);
	//if (pos >= N - 1)
	//	return N;

	auto const mask = ~std::bitset<N> {} << (pos + 1);
	x = x & mask;
	//return x.none() ? N : ctz(x);
	return ctz(x);
}

// Returns the position to the first set bit to the right of 'pos'
template <int N>
constexpr int bit_position_right(std::bitset<N> x, int const pos) {
	// assert(pos >= 0 && pos < N);
	if (pos == 0)
		return N;

	auto const mask = ~(~std::bitset<N> {} << pos);
	x = x & mask;
	return x.none() ? N : clz(x) - (N - 1);
}

// Find the nth set bit
template <int N>
constexpr int find_nth_set(std::bitset<N> const& bs, int n) {
	int i = find_first_set(bs);
	if (i == N)
		return N;

	int count = 1;
	while (count < n) {
		i = bit_position_left(bs, i);
		count += 1;
	}
	return i;
}

// Iterate the indices in a bitset
template <int N>
constexpr void bit_iterate_set(std::bitset<N> const& bs, auto fn) {
	for (int i = find_first_set(bs); i < N; i = bit_position_left(bs, i)) {
		fn(i);
	}
}

// Generate the subsets of the set bits in a bitset.
// O(2^n), where n is number of set bits
template <int N>
constexpr void bit_subsets(std::bitset<N> const& bs, auto fn) {
	for (auto subset = bs; subset.any(); subset = bit_minus_one(subset) & bs)
		fn(subset);
}

// Generate the subsets of the set bits in a bitset.
// O(2^n), where n is number of set bits
template <int N>
constexpr void bit_subsets(std::integral auto const bs, auto fn) {
	for (auto subset = bs; subset.any(); subset = (subset - 1) & bs)
		fn(subset);
}

// Generate the x-bits subsets of the set bits in the bit set.
// Only calls 'fn' if the subset has X bits set.
template <int N>
constexpr void bit_subsets_n(std::bitset<N> const& bs, int x, auto fn) {
	int num_subsets = binomial_coefficient(bs.count(), x);
	for (auto subset = bs; subset.any() && num_subsets; subset = bit_minus_one(subset) & bs) {
		if (subset.count() == x) {
			fn(subset);
			num_subsets -= 1;
		}
	}
}

// Generate the x-bits subsets of the set bits in the bit set.
// Valid subsets are passed asynchronously to 'fn' if the subset has X bits set.
template <int N, typename Fn>
constexpr void bit_subsets_n_par(std::bitset<N> const& bs, int x, Fn&& fn) {
	int num_subsets = binomial_coefficient(bs.count(), x);

	auto const producer = [=, subset = bs]() mutable {
		auto ret = std::optional<std::bitset<N>> {};
		while (subset.any() && num_subsets) {
			ret = subset;
			subset = bit_minus_one(subset) & bs;
			num_subsets -= 1;

			if (subset.count() == x)
				break;
		}
		return ret;
	};

	spmc(producer, std::forward<Fn>(fn));
}


// Find the largest stride of ones in the bitset
// fx.  000110010011111000
// would return 5
template <int N>
/*constexpr*/ int bit_has_n_contiguous_ones(std::bitset<N> bs, int n) {
	int i = ctz(bs);
	while (i != N) {
		bs >>= i;
		int const s = i;
		i = ctz(~bs);
		if (i - s >= n)
			return true;
		bs >>= (i+1);
		i = ctz(bs);
	}
	return false;
}

// Count the number of 'islands' in a bitset
// fx.  000110010011111000
// would return 3
template <int N>
/*constexpr*/ int bit_count_islands(std::bitset<N> bs) {
	int c = 0;
	int i = ctz(bs);
	while (i != N) {
		c += 1;
		bs >>= (i+1);
		i = ctz(~bs);
		bs >>= (i+1);
		i = ctz(bs);
	}
	return c;
}


} // namespace kg