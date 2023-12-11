import aoc;
using i64 = std::int64_t;
using pos2d = kg::pos2d<i64>;

constexpr auto expected_sample() { return std::make_tuple(374, 1030, 8410); }
constexpr auto expected_input() { return std::make_pair(10885634, 0); }

auto solve(auto const& input, i64 scale) {
	std::vector<i64> x_offset(input[0].size(), 1);
	std::vector<i64> y_offset(input.size(), 1);

	// Find galaxies. 
	// Set entries in y_offset/x_offset to zero where galaxies are found
	std::vector<pos2d> galaxies;
	for (i64 y = 0; y < input.size(); y += 1) {
		for (auto x = input[y].find('#'); x != input[y].npos; x = input[y].find('#', x + 1)) {
			x_offset[x] = 0;
			y_offset[y] = 0;
			galaxies.emplace_back(x, y);
		}
	}

	// Creates offsets.
	// Any remaining one's in these vectors indicate empty rows/columns.
	// from: 0010010010
	//   to: 0011122233
	std::inclusive_scan(x_offset.begin(), x_offset.end(), x_offset.begin());
	std::inclusive_scan(y_offset.begin(), y_offset.end(), y_offset.begin());

	// Correct galaxy positions
	for (pos2d& p : galaxies) {
		p.x += x_offset[p.x] * scale;
		p.y += y_offset[p.y] * scale;
	}

	i64 sum_shortest_path = 0;
	for (i64 i = 0; i < galaxies.size(); i++)
		for (i64 j = i + 1; j < galaxies.size(); j++)
			sum_shortest_path += galaxies[i].steps_to(galaxies[j]);

	return sum_shortest_path;
}
