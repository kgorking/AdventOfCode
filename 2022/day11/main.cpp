import aoc;

struct monkey {
	std::vector<std::size_t> items;
	char op;
	char test;
	char dest_true;
	char dest_false;
	size_t inspections = 0;

	bool operator>(monkey const& other) const {
		return inspections > other.inspections;
	}
};

template <bool Part1>
size_t calc_monkey_business(auto monkeys, int rounds) {
	// Calculate the product of all the monkeys divisors.
	std::uint64_t product = 1;
	for (monkey const& m : monkeys)
		product *= m.test;

	// Run the rounds
	for (int round = 1; round <= rounds; round += 1) {
		for (monkey& m : monkeys) {
			for (size_t worry : m.items) {
				// Calculate the worry
				if (m.op < 0) {
					// negative op -> multiply
					worry *= -m.op;
				} else if (m.op > 0) {
					// positive op -> addition
					worry += m.op;
				} else/*if (m.op == 0)*/ {
					// zero op -> square
					worry *= worry;
				}

				if constexpr (Part1) {
					// part 1: divide 'worry' by 3
					worry /= 3;
				} else {
					// part 2: wrap 'worry' with the product of divisors
					worry %= product;
				}

				// Send the item to another monkey
				char const destination = ((worry % m.test) == 0) ? m.dest_true : m.dest_false;
				monkeys[destination].items.push_back(worry);
			}

			// Count the inspections and clear the item vector
			m.inspections += m.items.size();
			m.items.clear();
		}
	}

	std::array<monkey, 2> top_monkeys;
	std::ranges::partial_sort_copy(monkeys, top_monkeys, std::greater{});
	return top_monkeys[0].inspections * top_monkeys[1].inspections;
}

int main() {
	const auto input = std::to_array<monkey>({
		#include "input.txt"
	});

	std::cout << "Part 1: " << calc_monkey_business<true>(input, 20) << '\n';
	std::cout << "Part 2: " << calc_monkey_business<false>(input, 10000);
}
