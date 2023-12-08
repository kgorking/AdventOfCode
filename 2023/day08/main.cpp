import aoc;

using node_map = std::map<std::string_view, std::array<std::string_view, 2>>;

struct input_t {
	std::string_view directions;
	node_map nodes;
};

#include "input.txt"

auto solve(bool p2) {
	auto distance = [&](std::string_view node) {
		std::size_t steps = 0;
		while (node.back() != 'Z') {
			char const c = input.directions[steps % input.directions.size()];
			node = input.nodes.at(node)[c == 'R'];
			steps += 1;
		}
		return steps;
	};

	auto is_start_node = [p2](auto n) { return (!p2) ? n == "AAA" : n.back() == 'A'; };
	auto start_nodes = input.nodes | std::views::keys | std::views::filter(is_start_node);

	auto const lcm = [](auto... a) { return std::lcm(a...); };
	return std::transform_reduce(start_nodes.begin(), start_nodes.end(), std::size_t{1}, lcm, distance);
}

int main() {
	std::cout << "Part 1: " << solve(false) << '\n';
	std::cout << "Part 2: " << solve(true) << '\n';
}
