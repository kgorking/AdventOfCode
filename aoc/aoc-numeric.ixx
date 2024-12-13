export module aoc:numeric;
import std;

export namespace kg {

// two equations with two unknowns
std::pair<int, int> solve(int a1, int b1, int c1, int a2, int b2, int c2) {
	int y = (a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1);
	int x = (b2 * c1 - b1 * c2) / (a1 * b2 - a2 * b1);
	return std::pair { x, y };
}

// Create a histogram from the container
// Returns a pair of std::map and the maximum histogram value
template <std::ranges::input_range Cont, typename KeyType = Cont::value_type>
auto histogram(Cont const& container) -> std::pair<std::map<KeyType, int>, int> {
	int max = 0;
	std::map<KeyType, int> map;
	for (auto const& v : container)
		max = std::max(max, ++map[v]);
	return {map, max};
}

// Create a histogram from the container
// Returns a vector
template <std::ranges::input_range Cont, typename KeyType = Cont::value_type>
auto histogram_vec(Cont const& container) -> std::vector<KeyType> {
	std::vector<KeyType> map(std::numeric_limits<KeyType>::max());
	for (auto const& v : container)
		++map[v];
	return map;
}
} // namespace kg
