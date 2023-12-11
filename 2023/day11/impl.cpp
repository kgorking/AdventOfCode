import aoc;
using i64 = std::int64_t;
using pos2d = kg::pos2d<i64>;

constexpr auto expected_sample() { return std::make_pair(374, 82000210); }
constexpr auto expected_input() { return std::make_pair(10885634, 707505470642); }

constexpr auto solve(auto const& input, i64 expansion) {
	// Initialize vectors to the expansion factor
	std::vector<i64> actual_x(input[0].size(), expansion);
	std::vector<i64> actual_y(input.size(), expansion);

	// Find galaxies.
	// Set entries in actual_y/actual_x to '1' where galaxies are found
	std::vector<pos2d> galaxies;
	for (i64 y = 0; y < input.size(); y += 1) {
		for (auto x = input[y].find('#'); x != input[y].npos; x = input[y].find('#', x + 1)) {
			actual_x[x] = 1;
			actual_y[y] = 1;
			galaxies.emplace_back(x, y);
		}
	}

	// Calculate the actual (x,y) positions
	std::inclusive_scan(actual_x.begin(), actual_x.end(), actual_x.begin());
	std::inclusive_scan(actual_y.begin(), actual_y.end(), actual_y.begin());

	// Correct galaxy positions
	for (pos2d& p : galaxies) {
		p.x = actual_x[p.x];
		p.y = actual_y[p.y];
	}

	// Calculate the shortest path between galaxies
	i64 sum_shortest_path = 0;
	for (i64 i = 0; i < galaxies.size(); i++)
		for (i64 j = i + 1; j < galaxies.size(); j++)
			sum_shortest_path += galaxies[i].steps_to(galaxies[j]);

	return sum_shortest_path;
}

constexpr auto part1(auto const& input) {
	return solve(input, 2);
}

constexpr auto part2(auto const& input) {
	return solve(input, 1'000'000);
}

constexpr auto solve(auto const& input) {
	return std::make_pair(part1(input), part2(input));
}

constexpr auto get_sample_input() {
	return std::to_array<std::string_view>({
		#include "sample_input.txt"
	});
}

constexpr auto get_actual_input() {
	return std::to_array<std::string_view>({
		#include "input.txt"
	});
}
