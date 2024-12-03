export module aoc.day03;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unittests.
export constexpr auto expected_sample = std::make_pair(161, 48);
export constexpr auto expected_input = std::make_pair(183788984, 62098619);

using namespace std::string_view_literals;
using operator_queue = std::priority_queue<std::size_t, std::vector<std::size_t>, std::greater<>>;

std::string_view extract_number(std::size_t const start, std::string_view s) {
	std::size_t const end = s.find_first_not_of("0123456789", start);
	return s.substr(start, end - start);
}

template <std::size_t N>
constexpr std::uint64_t sum_products(std::array<std::string_view, N> ops, std::string_view program) {
	bool enabled = true;
	std::uint64_t sum = 0;

	// Load the queue with the first indices of the operators
	operator_queue queue;
	for (std::string_view op : ops)
		queue.push(program.find(op));

	while (!queue.empty()) {
		// Determine the operator
		std::size_t const op_begin = queue.top();
		std::size_t const op_end = program.find('(', op_begin) + 1; // +1 to include the '('
		std::string_view const op = program.substr(op_begin, op_end - op_begin);

		// Reload the queue with the next instance of the current operator
		queue.pop();
		std::size_t const next_op = program.find(op, op_end);
		if (next_op != std::string_view::npos)
			queue.push(next_op);

		// If it's not a multiply, determine enabled state
		if (op[0] != 'm') {
			enabled = (op == "do("sv);
			continue;
		}

		// First number to multiply
		std::size_t const a_start = op_end;
		std::string_view const a = extract_number(a_start, program);
		if (a.empty() || program[a_start + a.size()] != ',')
			continue;

		// Second number to multiply
		std::size_t const b_start = op_end + a.size() + 1;
		std::string_view const b = extract_number(b_start, program);
		if (b.empty() || program[b_start + b.size()] != ')')
			continue;

		// Accumulate the product if enabled
		if (enabled)
			sum += std::atoi(a.data()) * std::atoi(b.data());
	}

	return sum;
}

export auto part1(auto&& input) {
	auto const operators = std::to_array({"mul("sv});
	return sum_products(operators, input);
}

export auto part2(auto&& input) {
	auto const operators = std::to_array({"mul("sv, "do("sv, "don't("sv});
	return sum_products(operators, input);
}
