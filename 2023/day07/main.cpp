import aoc;

using card = char;
using hand = std::basic_string_view<card>;

struct hand_and_bid {
	hand hand;
	int bid;
	int type[2] = {0, 0};
};

// Determine the type of a hand, including with jokers
std::pair<int, int> types(hand const& h) {
	// Count the different cards and how many of them there are
	int max_val = 0;
	std::map<card, int> map;
	for (card c : h)
		max_val = std::max(max_val, ++map[c]);

	// Determine the type from the number of cards and their counts
	// (The following lookups are a switch statement flattened into lookup tables)
	int const type_lookup[5] = {6, 5 - (3 == max_val), 3 - (2 == max_val), 1, 0};
	int const type_p1 = type_lookup[map.size() - 1];

	// Consider potential jokers for part 2
	int type_p2 = type_p1;
	int const num_jokers = map['J'];
	if (num_jokers > 0) {
		int const joker_offsets[7] = {1, 2, num_jokers + 1, 2, 2, 1, 0};
		type_p2 += joker_offsets[type_p1];
	}

	return {type_p1, type_p2};
}

// Convert a card into a comparable value
template <bool P2>
int card_value(card c) {
	return std::string_view{"J23456789TJQKA"}.find_first_of(c, !P2);
}

// Compare two hands
template <bool P2>
bool compare(hand_and_bid const& hb1, hand_and_bid const& hb2) {
	// Compare type if possible
	if (hb1.type[P2] != hb2.type[P2])
		return hb1.type[P2] < hb2.type[P2];

	// Use the card values for comparison
	auto const [it1, it2] = std::ranges::mismatch(hb1.hand, hb2.hand);
	return card_value<P2>(*it1) < card_value<P2>(*it2);
}

// Solves the input.
// Order it by hand type and then add up the winnings
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

	// Pre-determine the hand types for faster sorting
	for (hand_and_bid& hb : input) {
		std::tie(hb.type[0], hb.type[1]) = types(hb.hand);
	}

	std::cout << "Part 1: " << solve<false>(input) << '\n';
	std::cout << "Part 2: " << solve<true>(input) << '\n';
}
