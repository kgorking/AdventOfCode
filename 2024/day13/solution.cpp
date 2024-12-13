export module aoc.day13;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(480ll, 875318608908ll);
export constexpr auto expected_input = std::make_pair(34787ll, 85644161121698ll);

template<typename i64 = std::int64_t>
std::pair<i64, i64> solve(i64 a1, i64 b1, i64 c1, i64 a2, i64 b2, i64 c2) {
	i64 y = (a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1);
	i64 x = (b2 * c1 - b1 * c2) / (a1 * b2 - a2 * b1);
	return std::pair { x, y };
}

export auto part1(auto&& input) {
	std::int64_t sum = 0;
	for (auto eq : input) {
		auto [a, b, c, d, e, f] = eq;

		auto [x, y] = solve(a, b, c, d, e, f);
		bool const valid_solution = (a * x + b * y == c) && (d * x + e * y == f);
		if (valid_solution && x >= 0 && y >= 0 && x <= 100 && y <= 100) {
			sum += x * 3 + y;
		}
	}

	return sum;
}

export constexpr auto part2(auto&& input) {
	std::int64_t sum = 0;
	for (auto eq : input) {
		auto [a, b, c, d, e, f] = eq;

		c += 10000000000000ll;
		f += 10000000000000ll;

		auto [x, y] = solve(a, b, c, d, e, f);
		bool const valid_solution = (a * x + b * y == c) && (d * x + e * y == f);
		if (valid_solution && x >= 0 && y >= 0) {
			sum += x * 3 + y;
		}
	}

	return sum;
}
