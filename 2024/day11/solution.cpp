export module aoc.day11;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(55312, 65601038650482LL);
export constexpr auto expected_input = std::make_pair(189092, 224869647102559LL);

int num_digits(std::int64_t x) {
	return static_cast<int>(log10(x)) + 1;
}

std::int64_t calc_spawn_iterative(auto&& input, int steps) {
	std::unordered_map<std::int64_t, std::int64_t> curr, next;

	for (std::int64_t i : input)
		curr.insert({ i, 1 });

	while (steps-- > 0) {
		for (auto const [stone_val, count] : curr) {
			if (stone_val == 0) {
				next[1] += count;
			} else if (int d = num_digits(stone_val); !(d & 1)) {
				std::int64_t const half_pow = std::pow(10, d / 2);
				std::int64_t const l = stone_val / half_pow;
				std::int64_t const r = stone_val - (l * half_pow);
				next[l] += count;
				next[r] += count;
			} else {
				next[2024 * stone_val] += count;
			}
		}
		curr.swap(next);
		next.clear();
	}

	return kg::sum(curr | std::views::values);
}

export auto part1(auto&& input) {
	return calc_spawn_iterative(input, 25);
}

export auto part2(auto&& input) {
	return calc_spawn_iterative(input, 75);
}
