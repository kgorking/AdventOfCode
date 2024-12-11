export module aoc.day11;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(55312, 65601038650482LL);
export constexpr auto expected_input = std::make_pair(189092, 224869647102559LL);

int num_digits(std::int64_t x) {
	return static_cast<int>(log10(x)) + 1;
}

std::int64_t calc_spawn(std::int64_t stone_val, int step, auto& cache) {
	if (step == 0)
		return 1;

	std::int64_t const key = (stone_val << 8) | step;
	std::int64_t res = cache[key];
	if (res == 0) {
		if (stone_val == 0) {
			res = calc_spawn(1, step - 1, cache);
		} else if (int d = num_digits(stone_val); !(d & 1)) {
			std::int64_t const half_pow = std::pow(10, d / 2);
			std::int64_t const l = stone_val / half_pow;
			std::int64_t const r = stone_val - (l * half_pow);
			res = calc_spawn(l, step - 1, cache) + calc_spawn(r, step - 1, cache);
		} else {
			res = calc_spawn(2024 * stone_val, step - 1, cache);
		}
	}

	cache[key] = res;
	return res;
}

export auto part1(auto&& input) {
	std::unordered_map<std::int64_t, std::int64_t> cache;
	return kg::sum(input | std::views::transform([&cache](std::int64_t i) { return calc_spawn(i, 25, cache); }));
}

export auto part2(auto&& input) {
	std::unordered_map<std::int64_t, std::int64_t> cache;
	return kg::sum(input | std::views::transform([&cache](std::int64_t i) { return calc_spawn(i, 75, cache); }));
}
