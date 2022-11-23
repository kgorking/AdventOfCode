#include <array>
#include <iostream>
#include <string_view>
#include <unordered_map>

using packed_pair = short;

packed_pair pack_pair(char c1, char c2) {
	return (c1 << 8) | c2;
}

// Given AB and C, returns {AC, CB}
std::pair<packed_pair, packed_pair> insert_char_in_pair(packed_pair const pp, char insert) {
	packed_pair const left = (pp & 0xFF00) | insert;
	packed_pair const right = (insert << 8) | (pp & 0x00FF);

	return {left, right};
}

struct puzzle_input {
	std::string_view polymer_template;
	std::unordered_map<packed_pair, char> rules; // AB -> C, ((A<<8) | B) -> C
};

puzzle_input const input{
#include "input.txt"
};

std::pair<int64_t, int64_t> find_min_max(std::ranges::range auto const& rng) {
	int64_t min = std::numeric_limits<int64_t>::max(), max = std::numeric_limits<int64_t>::min();

	for (int64_t const count : rng) {
		if (count > 0) { // Some chars are never touched and remain zero
			min = std::min(min, count);
			max = std::max(max, count);
		}
	}

	return {min, max};
}

int main() {
	std::unordered_map<packed_pair, int64_t> pair_count;
	std::array<int64_t, 'Z' - 'A'> char_count;
	char_count.fill(0);

	// Fill the counters from the starting polymer template
	for (size_t i = 0; i < input.polymer_template.size() - 1; ++i) {
		packed_pair const pp = pack_pair(input.polymer_template[i], input.polymer_template[i + 1]);

		pair_count[pp] += 1;
		char_count[input.polymer_template[i] - 'A'] += 1;
	}
	char_count[input.polymer_template.back() - 'A'] += 1;

	// Run the steps to expand the polymer
	for (int step = 1; step <= 40; ++step) {
		std::unordered_map<packed_pair, int64_t> next_pair_count;

		// Expand pairs to create new pairs
		for (auto const [pp, count] : pair_count) {
			char const insert = input.rules.at(pp);

			auto const [left, right] = insert_char_in_pair(pp, insert);

			next_pair_count[left] += count;
			next_pair_count[right] += count;
			char_count[insert - 'A'] += count;
		}

		// Update the pairs to the new batch
		pair_count = std::move(next_pair_count);

		// Print solution to part 1
		if (step == 10) {
			auto const [min, max] = find_min_max(char_count);
			std::cout << "Part 1: " << (max - min) << '\n';
		}
	}

	auto const [min, max] = find_min_max(char_count);
	std::cout << "Part 2: " << (max - min) << '\n';
}
