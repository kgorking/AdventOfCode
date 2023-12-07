import aoc;

using card = char;
using hand = std::basic_string_view<card>;

struct hand_and_bid {
	hand hand;
	int bid;
	int strength_p1 = 0;
	int strength_p2 = 0;
};

// Determine the strength of a hand. Considers jokers if P2 is true
template <bool P2>
int strength(hand_and_bid const& hb) {
	// Count the different cards and how many of them there are
	std::map<card, unsigned char> map;
	for (card c : hb.hand) {
		map[c] += 1;
	}

	// Count potential jokers for part 2
	int const jokers = (P2 && map.contains('J')) ? map['J'] : 0;

	// Determine the type, and shift it if jokers are present
	switch (map.size()) {
	case 5: { // high card (1)
		// if one card is joker, it becomes a pair (2)
		return 1 + jokers;
	}
	case 4: { // one pair (2)
		// JJXYZ -> three of a kind (4)
		// JXXYZ -> three of a kind (4)
		return 2 + (jokers ? 2 : 0);
	}
	case 3: {
		// two pair (3)
		if (2 == *std::ranges::max_element(map | std::views::values)) {
			// JXXYY -> full house
			// JJXXY -> four of a kind
			return 3 + (jokers ? jokers + 1 : 0);
		}

		// three of a kind (4)
		// JJJXY -> four of a kind (6)
		// JXXXY -> four of a kind (6)
		return 4 + (jokers ? 2 : 0);
	}
	case 2: {
		// full house (5)
		if (3 == *std::ranges::max_element(map | std::views::values)) {
			// JJJXX -> five of a kind (7)
			// JJXXX -> five of a kind (7)
			return 5 + (jokers ? 2 : 0);
		}

		// four of a kind (6)
		// JXXXX -> five of a kind (7)
		// JJJJX -> five of a kind (7)
		return 6 + (jokers ? 1 : 0);
	}
	default: // case 1:
		// five of a kind (7)
		return 7;
	}
}

// Convert a card into a comparable value
template <bool P2>
int card_value(card c) {
	switch (c) {
	case 'A': return 14;
	case 'K': return 13;
	case 'Q': return 12;
	case 'J': return P2 ? 1 : 11;
	case 'T': return 10;
	default:  return c - '0';
	}
}

// Compare strength of two hands
template <bool P2>
bool compare(hand_and_bid const& hb1, hand_and_bid const& hb2) {
	// Compare strength if possible
	int const str1 = P2 ? hb1.strength_p2 : hb1.strength_p1;
	int const str2 = P2 ? hb2.strength_p2 : hb2.strength_p1;
	if (str1 != str2)
		return str1 < str2;

	// Ignore identical cards
	int offset = 0;
	while (hb1.hand[offset] == hb2.hand[offset])
		offset += 1;

	// Use the card values for comparison
	return card_value<P2>(hb1.hand[offset]) < card_value<P2>(hb2.hand[offset]);
}

// Solves the input. Order it by hand strength and then add up the winnings
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
		hb.strength_p1 = strength<false>(hb);
		hb.strength_p2 = strength<true>(hb);
	}

	std::cout << "Part 1: " << solve<false>(input) << '\n';
	std::cout << "Part 2: " << solve<true>(input) << '\n';
}
