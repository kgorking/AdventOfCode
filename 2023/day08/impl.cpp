﻿import aoc;

using node_map = std::map<std::string_view, std::array<std::string_view, 2>>;

struct input_t {
	std::string_view directions;
	node_map nodes;
};

auto part1(auto const& input) {
	auto distance = [&](std::string_view node) {
		std::size_t steps = 0;
		while (node.back() != 'Z') {
			char const c = input.directions[steps % input.directions.size()];
			node = input.nodes.at(node)[c == 'R'];
			steps += 1;
		}
		return steps;
	};

	auto is_start_node = [](auto n) { return n == "AAA"; };
	auto start_nodes = input.nodes | std::views::keys | std::views::filter(is_start_node);

	auto lcm = std::lcm<std::size_t, std::size_t>;
	return std::transform_reduce(start_nodes.begin(), start_nodes.end(), 1ull, lcm, distance);
}

auto part2(auto const& input) {
	auto distance = [&](std::string_view node) {
		std::size_t steps = 0;
		while (node.back() != 'Z') {
			char const c = input.directions[steps % input.directions.size()];
			node = input.nodes.at(node)[c == 'R'];
			steps += 1;
		}
		return steps;
	};

	auto is_start_node = [](auto n) { return n.back() == 'A'; };
	auto start_nodes = input.nodes | std::views::keys | std::views::filter(is_start_node);

	auto lcm = std::lcm<std::size_t, std::size_t>;
	return std::transform_reduce(start_nodes.begin(), start_nodes.end(), 1ull, lcm, distance);
}

auto solve(auto const& input) {
	return std::make_pair(part1(input), part2(input));
}