export module aoc.day08;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(14, 34);
export constexpr auto expected_input = std::make_pair(301, 1019);

constexpr int max_node_types = ('z' - 'a') * 2 + 10;

int node_to_index(char c) {
	if (std::isdigit(c))
		return c - '0';
	else
		return std::isupper(c) ? 10 + (c - 'A') : 10 + ('Z' - 'A') + (c - 'a');
}

template<bool part1>
int count_antinodes(auto&& input) {
	std::array<std::vector<kg::pos2di>, max_node_types> antenna_locations;

	for (int y = 0; std::string_view line : input) {
		for (std::size_t x = line.find_first_not_of('.'); x != line.npos; x = line.find_first_not_of('.', x + 1)) {
			int const index = node_to_index(line[x]);
			antenna_locations[index].emplace_back(x, y);
		}
		y++;
	}

	std::set<kg::pos2di> antinodes;
	for (std::vector<kg::pos2di> const& locations : antenna_locations) {
		auto is_valid = [&](kg::pos2di p) { return p.y >= 0 && p.y < input.size() && p.x >= 0 && p.x < input[0].size(); };

		for (auto const [i, p1] : locations | std::views::enumerate) {
			for (int j = i + 1; j < locations.size(); j++) {
				kg::pos2di const p2 = locations[j];
				auto const dir = p2 - p1;

				if constexpr (part1) {
					kg::pos2di node1 = p1 - dir;
					kg::pos2di node2 = p1 + dir * 2;

					if (is_valid(node1))
						antinodes.insert(node1);
					if (is_valid(node2))
						antinodes.insert(node2);
				} else {
					kg::pos2di node1 = p2 - dir;
					kg::pos2di node2 = p1 + dir;

					while (is_valid(node1)) {
						antinodes.insert(node1);
						node1 -= dir;
					}
					while (is_valid(node2)) {
						antinodes.insert(node2);
						node2 += dir;
					}
				}
			}
		}
	}

	return antinodes.size();
}

export auto part1(auto&& input) {
	return count_antinodes<true>(input);
}

export auto part2(auto&& input) {
	return count_antinodes<false>(input);
}
