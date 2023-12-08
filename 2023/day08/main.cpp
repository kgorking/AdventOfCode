import aoc;

struct node {
	std::string_view id;
	std::string_view lr[2];
};

template <int N>
struct input_t {
	std::string_view directions;
	std::array<node, N> nodes;
};

// Pre-process the data. Convert from logN to constant lookups
template <bool Part2>
auto preprocess() {
	#include "input.txt"

	// Assign index to name
	std::map<std::string_view, int> name_to_index;
	for (int i = 0; i < input.nodes.size(); i++) {
		auto const id = input.nodes[i].id;
		// end-nodes have negative index
		name_to_index[id] = (id.back() == 'Z') ? -i : +i;
	}

	// Find starting indicies
	std::vector<short> start;
	if constexpr (!Part2) {
		start.push_back(name_to_index["AAA"]);
	}

	// Convert left/right names to indexes
	std::array<short[2], input.nodes.size()> lr;
	for (int i = 0; auto const& node : input.nodes) {
		lr[i][0] = name_to_index[node.lr[0]];
		lr[i][1] = name_to_index[node.lr[1]];
		i += 1;

		if constexpr (Part2) {
			if (node.id.back() == 'A') {
				start.push_back(name_to_index[node.id]);
			}
		}
	}

	return std::make_tuple(start, input.directions, lr);
}

auto solve(auto const& input) {
	auto const& [starts, directions, lookup] = input;

	auto distance = [&](short node) {
		std::size_t steps = 0;
		while (node >= 0) {
			char const c = directions[steps];
			node = lookup[node][c == 'R'];
			steps += 1;
		}
		return steps;
	};

	auto const lcm = [](auto... a) { return std::lcm(a...); };
	return std::transform_reduce(starts.begin(), starts.end(), std::size_t{1}, lcm, distance);
}

int main() {
	std::cout << "Part 1: " << solve(preprocess<false>()) << '\n';
	std::cout << "Part 2: " << solve(preprocess<true>()) << '\n';
}
