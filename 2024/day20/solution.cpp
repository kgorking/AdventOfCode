export module aoc.day20;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(0, 0);
export constexpr auto expected_input = std::make_pair(0, 0);

template<int N>
using input_t = std::array<std::string_view, N>;

struct cheat {
	kg::pos2di s, e;
};

export template <int N>
auto part1(input_t<N> const& input) {
	kg::grid map { input };

	kg::pos2di const S = map.find('S');
	kg::pos2di const E = map.find('E');

	// Walk the path and look for places to cheat
	std::vector<cheat> cheats;
	kg::pos2di p = S;
	while(p != S) {
	}

	return 0;
}

export template <int N>
auto part2(input_t<N> const& input) {
	return 0;
}
