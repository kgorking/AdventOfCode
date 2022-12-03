#include <array>
#include <iostream>
#include <numeric>
#include <string_view>
#include <bitset>

constexpr auto input = std::to_array<std::string_view>({
	#include "input.txt"
});

// Bitset to represent the rucksack as two 52-bit compartments
using bitset = std::bitset<2 * 52>;

// Convert a charater to a bit in 0-51
constexpr bitset to_bit(char c) {
	if (c >= 'a')
		return bitset{1} << (c - 'a');
	else
		return bitset{1} << (c - 'A' + 26);
}

// Convert a rucksack of characters to a bitset
constexpr bitset to_bitset(std::string_view sv) {
	bitset bs = 0;
	auto const middle = sv.size() / 2;
	for (std::size_t i = 0, j = middle; i < middle; i++, j++) {
		bs |= to_bit(sv[i]);       // first compartment
		bs |= to_bit(sv[j]) << 52; // second compartment
	}
	return bs;
}

// Calculate an items priority
constexpr int calc_item_priority(bitset bs) {
	// Count the high bits first
	int prio = std::countr_zero((bs >> 64).to_ullong());
	if (prio == 64) {
		// Nothing found in the high bits, so return the count of the low bits
		return 1 + std::countr_zero(bs.to_ullong());
	} else {
		// Item found in high bits, so return it plus the count of the low bits (64)
		return 1 + 64 + prio;
	}
}

// Returns a bitset with all items from both compartments
constexpr bitset merge_bitset_compartments(bitset bs) {
	bitset const comp1 = (bs << 52) >> 52;
	bitset const comp2 = (bs >> 52);
	return comp1 | comp2;
}

// Find the item priority of an item that exists in both compartments
constexpr int find_common_item_priority(std::string_view sv) {
	bitset const bs = to_bitset(sv);

	bitset const comp1 = (bs << 52) >> 52;
	bitset const comp2 = (bs >> 52);

	bitset const common_item = comp1 & comp2;
	return calc_item_priority(common_item);
}

int main() {
	// Part 1
	int priority_sum = 0;
	for (auto sv : input)
		priority_sum += find_common_item_priority(sv);
	std::cout << priority_sum << '\n';

	// Part 2
	int badge_priority_sum = 0;
	for (std::size_t i = 0; i < input.size() / 3; i++) {
		bitset const badge =
			merge_bitset_compartments(to_bitset(input[i*3+0])) &
			merge_bitset_compartments(to_bitset(input[i*3+1])) &
			merge_bitset_compartments(to_bitset(input[i*3+2]));
		
		badge_priority_sum += calc_item_priority(badge);
	}
	std::cout << badge_priority_sum << '\n';
}
