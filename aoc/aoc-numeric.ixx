export module aoc:numeric;
import std;

export namespace kg {

// Create a histogram from the container
// Returns a pair of std::map and the maximum histogram value
template <std::ranges::input_range Cont, typename KeyType = Cont::value_type>
std::pair<std::map<KeyType, int>, int> histogram(Cont const& container) {
	int max = 0;
	std::map<KeyType, int> map;
	for (auto const& v : container)
		max = std::max(max, ++map[v]);
	return {map, max};
}
} // namespace kg
