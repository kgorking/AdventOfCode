import aoc;

using card = char;
using hand = std::basic_string_view<card>;

struct hand_and_bid {
	hand hand;
	int bid;
	int strength[2] = {0, 0};
};

// Determine the strength of a hand. Considers jokers if P2 is true
template <bool P2>
int strength(hand_and_bid const& hb) {
	// Count the different cards and how many of them there are
	int max_val = 0;
	std::map<card, int> map;
	for (card c : hb.hand) {
		map[c] += 1;
		max_val = std::max(max_val, map[c]);
	}

	// Determine the type from the number of cards and their counts
	// (The following lookups are a switch statement flattened into lookup tables)
	int const type_lookup[5] = {6, 5 - (3 == max_val), 3 - (2 == max_val), 1, 0};
	int type = type_lookup[map.size() - 1];

	// Consider potential jokers for part 2
	if constexpr (P2) {
		int const num_jokers = map['J'];
		if (num_jokers > 0) {
			int const joker_offsets[7] = {1, 2, num_jokers + 1, 2, 2, 1, 0};
			type += joker_offsets[type];
		}
	}

	return type;
}

// Convert a card into a comparable value
template <bool P2>
int card_value(card c) {
	auto const index = std::string_view{"123456789TJQKA"}.find_first_of(c);
	return (P2 && index == 10) ? 0 : index;
}

// Compare two hands
template <bool P2>
bool compare(hand_and_bid const& hb1, hand_and_bid const& hb2) {
	// Compare strength if possible
	if (hb1.strength[P2] != hb2.strength[P2])
		return hb1.strength[P2] < hb2.strength[P2];

	// Use the card values for comparison
	auto const [it1, it2] = std::ranges::mismatch(hb1.hand, hb2.hand);
	return card_value<P2>(*it1) < card_value<P2>(*it2);
}

// Solves the input.
// Order it by hand strength and then add up the winnings
template <bool P2>
int solve(auto& input) {
	std::ranges::sort(input, compare<P2>);
	return std::transform_reduce(input.begin(), input.end(), 0, std::plus<>{}, [rank = 1](hand_and_bid const& hb) mutable {
		return hb.bid * rank++;
	});
}

int main() {
	auto input = std::to_array<hand_and_bid>({
		#include "input.txt"
	});

	// Pre-determine the types for faster sorting
	for (hand_and_bid& hb : input) {
		hb.strength[false] = strength<false>(hb);
		hb.strength[true] = strength<true>(hb);
	}

	std::cout << "Part 1: " << solve<false>(input) << '\n';
	std::cout << "Part 2: " << solve<true>(input) << '\n';
}
