export module aoc:numeric;
import std;

export namespace kg {

// two equations with two unknowns
constexpr std::pair<std::int64_t, std::int64_t> solve(std::int64_t a1, std::int64_t b1, std::int64_t c1, std::int64_t a2, std::int64_t b2, std::int64_t c2) {
	std::int64_t const y = (a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1);
	std::int64_t const x = (b2 * c1 - b1 * c2) / (a1 * b2 - a2 * b1);
	return { x, y };
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
