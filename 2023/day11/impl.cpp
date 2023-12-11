import aoc;
using pos2d = kg::pos2d<int>;

constexpr auto expected_sample() { return std::make_pair(374, 0); }
constexpr auto expected_input() { return std::make_pair(10885634, 0); }

auto solve(auto&& input) {
	std::vector<int> y_offset(input.size(), 1);
	std::vector<int> x_offset(input[0].size(), 1);
	std::vector<pos2d> galaxies;

	// Find galaxies. Set entries in y_offset/x_offset to zero
	// where galaxies are found
	for (int y = 0; std::string_view line : input) {
		for (auto x = line.find('#'); x != line.npos; x = line.find('#', x + 1)) {
			y_offset[y] = 0;
			x_offset[x] = 0;
			galaxies.emplace_back(x, y);
		}
		y += 1;
	}

	// Sum up offsets
	std::inclusive_scan(y_offset.begin(), y_offset.end(), y_offset.begin());
	std::inclusive_scan(x_offset.begin(), x_offset.end(), x_offset.begin());

	// Correct galaxy positions
	for (pos2d& p : galaxies) {
		p.x += x_offset[p.x];
		p.y += y_offset[p.y];
	}

	//std::cout << "9  = " << galaxies[4].steps_to(galaxies[8]) << '\n';
	//std::cout << "15 = " << galaxies[0].steps_to(galaxies[6]) << '\n';
	//std::cout << "17 = " << galaxies[2].steps_to(galaxies[5]) << '\n';
	//std::cout << "5  = " << galaxies[7].steps_to(galaxies[8]) << '\n';

	int sum_shortest_path = 0;
	for (int i = 0; i < galaxies.size(); i++) {
		pos2d g1 = galaxies[i];
		for (int j = i + 1; j < galaxies.size(); j++) {
			pos2d g2 = galaxies[j];
			sum_shortest_path += g1.steps_to(g2);
		}
	}

	return std::make_pair(sum_shortest_path, 0);
}
