export module aoc:views;
import std;

using namespace std::placeholders;

namespace kg {
export auto lookup(auto&& v, int I, auto... Is) {
	if constexpr (sizeof...(Is) > 0) {
		return lookup(v[I], Is...);
	}
	else
		return v[I];
}
}

namespace kg::views {

export constexpr auto pair_zip_transform = std::bind(std::views::zip_transform, _3, _1, _2);
export constexpr auto indexed_transform = std::bind(std::views::zip_transform, _2, std::views::iota(0), _1);

// TODO
// at
// gather?
// pairwise_diff etc.

export constexpr auto filter_eq = [](auto&& val, std::integral auto... indices) {
	if constexpr (0 == sizeof...(indices))
		return std::views::filter(std::bind_front(std::equal_to {}, val));
	else {
		return std::views::filter([=](auto&& in) {
			return val == lookup(in, indices...);
			});
	}
};

// TODO avoid copying into arrays
export template <int N, int M>
constexpr auto matrix = std::views::adjacent_transform<N>([](auto&&... rows) {
	auto as_array = [](auto&&... args) {
		return std::array { args... };
	};
	return std::views::zip_transform(as_array, (rows | std::views::adjacent_transform<M>(as_array))...);
}) | std::views::join;

} // namespace kg::views
