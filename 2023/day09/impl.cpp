import aoc;

auto solve(auto const& input) {
	std::int64_t total_history[2] = {0, 0};

	using history = decltype(input[0]);

	for (history const& h : input) {
		auto values = h;
		auto vbegin = values.begin();
		auto front_history = history{h.front()};
		auto fh_it = ++front_history.begin();

		while (true) {
			std::adjacent_difference(vbegin, values.end(), vbegin);
			if (std::all_of(++vbegin, values.end(), std::bind_front(std::equal_to<>{}, 0)))
				break;

			*fh_it++ = *vbegin;
			total_history[0] += values.back();
		}

		auto val = 0;
		do { 
			val = *--fh_it - val;
		} while (fh_it != front_history.begin());

		total_history[0] += h.back();
		total_history[1] += val;
	}

	return std::make_pair(total_history[0], total_history[1]);
}

constexpr auto part1(auto const& input) {
	return solve(input).first;
}

constexpr auto part2(auto const& input) {
	return solve(input).second;
}

constexpr auto expected_sample() { return std::make_pair(114, 2); }
constexpr auto expected_input() { return std::make_pair(1696140818, 1152); }

constexpr auto get_sample_input() {
	return std::to_array<std::array<std::int64_t, 6>>({
		#include "sample_input.txt"
	});
}

constexpr auto get_actual_input() {
	return std::to_array<std::array<std::int64_t, 21>>({
		#include "input.txt"
	});
}

