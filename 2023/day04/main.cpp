import aoc;

//using card = std::pair<std::array<char, 5>, std::array<char, 8>>;
using card = std::pair<std::array<char, 10>, std::array<char, 25>>;

/*constexpr*/ auto input = std::to_array<card>({
#include "input.txt"
});

int part1() {
	auto const xform = [](card& c) {
		// Find the number or winning numbers
		card::first_type out;
		auto const set_result = std::ranges::set_intersection(c.first, c.second, out.begin());
		auto const winners = std::distance(out.begin(), set_result.out);

		// Calculate the points
		return std::pow(2, winners - 1);
	};

	return std::transform_reduce(input.begin(), input.end(), 0, std::plus<>{}, xform);
}

int part2() {
	std::array<int, input.size()> scratchcard_count;
	scratchcard_count.fill(1);

	auto const xform = [&, card_index = 0](card& c, int count) mutable {
		// Find the amount or winning numbers
		card::first_type out;
		auto const set_result = std::ranges::set_intersection(c.first, c.second, out.begin());
		auto winners = std::distance(out.begin(), set_result.out);

		// Add out newly won cards to the pile
		while (winners > 0 && (card_index + winners) < scratchcard_count.size()) {
			scratchcard_count[card_index + winners] += count;
			winners -= 1;
		}

		// Return the current number of cards and advance the index
		return scratchcard_count[card_index++];
	};

	return std::transform_reduce(input.begin(), input.end(), scratchcard_count.begin(), 0, std::plus<>{}, xform);
}

int main() {
	// Pre-process
	for (card& c : input) {
		// Sort the two sets
		std::ranges::sort(c.first);
		std::ranges::sort(c.second);
	}

	// Part 1
	std::cout << "Part 1: " << part1() << '\n';

	// Part 2
	std::cout << "Part 2: " << part2() << '\n';
}
