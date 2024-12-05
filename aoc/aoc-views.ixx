export module aoc:views;
import std;
import :matrix;

using namespace std::placeholders;

export namespace kg::views {
constexpr auto pair_zip_transform = std::bind(std::views::zip_transform, _3, _1, _2);
constexpr auto indexed_transform = std::bind(std::views::zip_transform, _2, std::views::iota(0), _1);

// TODO
// at
// gather?
// filter_eq etc. + indexes
// pairwise_diff etc.

// TODO avoid copying into arrays
template <int N, int M>
constexpr auto matrix = std::views::adjacent_transform<N>([](auto&&... rows) {
	auto as_array = [](auto&&... args) {
		return std::array { args... };
	};
	return std::views::zip_transform(as_array, (rows | std::views::adjacent_transform<M>(as_array))...);
}) | std::views::join;

} // namespace kg::views
