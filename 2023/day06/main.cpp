import aoc;

auto calc_distance(std::int64_t race_time, std::int64_t hold_time) -> std::int64_t {
	std::int64_t const travel_time = race_time - hold_time;
	std::int64_t const speed = hold_time;

	return speed * travel_time;
}

auto find_min_hold_times(std::int64_t race_time, std::int64_t record_dist) -> std::pair<std::int64_t, std::int64_t> {
	// Quadratic formula to find hold times
	std::int64_t const h1 = (race_time - std::sqrt((race_time * race_time) - (4 * record_dist))) / 2;
	std::int64_t const h2 = (race_time + std::sqrt((race_time * race_time) - (4 * record_dist))) / 2;

	std::int64_t const d1 = calc_distance(race_time, h1);
	std::int64_t const d2 = calc_distance(race_time, h2);

	return {
		h1 + (d1 <= record_dist),
		h2 - (d2 <= record_dist)
	};
}

auto solve(auto const& input) {
	std::int64_t product = 1;

	for (int i = 0; i < input[0].size(); i++) {
		auto const [a, b] = find_min_hold_times(input[0][i], input[1][i]);
		product *= 1 + (b-a);
	}

	return product;
}

int main() {
	constexpr auto input1 = std::to_array({
		#include "input1.txt"
	});
	constexpr auto input2 = std::to_array({
		#include "input2.txt"
	});

	std::cout << "Part 1: " << solve(input1) << '\n';
	std::cout << "Part 2: " << solve(input2) << '\n';
}
