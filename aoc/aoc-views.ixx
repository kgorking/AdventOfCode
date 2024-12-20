export module aoc:views;
import std;
import :math;
import :pos2d;

using namespace std::placeholders;

namespace kg::views {

export constexpr auto pair_zip_transform = std::bind(std::views::zip_transform, _3, _1, _2);
export constexpr auto indexed_transform = std::bind(std::views::zip_transform, _2, std::views::iota(0), _1);

export constexpr auto signbit = std::views::transform([](auto&& v) { return std::signbit(v); });
export constexpr auto abs = std::views::transform(kg::abs);

// TODO
// at
// gather?

// Takes two ranges and outputs the difference between them
export constexpr auto zip_diff = std::bind_front(std::views::zip_transform, [](auto&& l, auto&& r) {
	return std::abs(l - r); });

// Applies the function to the input, with optional additional arguments
export constexpr auto filter_fn = [](auto&& fn, auto&& ...args) {
	return std::views::filter([=](auto&& v) { return fn(v, args...); });
};

// Filters if the value is equal to the input.
// If the input is a range/container, use 'indices' to index into it.
export constexpr auto filter = [](auto&& val, auto Comp, auto Proj = std::identity {}) {
	return std::views::filter([=](auto&& in) { return Comp(val, std::invoke(Proj, in));
	});
};
export constexpr auto filter_equal = [](auto&& val, auto Proj = std::identity {}) {
	return filter(val, std::equal_to {}, Proj);
};
export constexpr auto filter_not_equal = [](auto&& val, auto Proj = std::identity {}) {
	return filter(val, std::not_equal_to {}, Proj);
};
export constexpr auto filter_greater = [](auto&& val, auto Proj = std::identity {}) {
	return filter(val, std::greater {}, Proj);
};
export constexpr auto filter_less = [](auto&& val, auto Proj = std::identity {}) {
	return filter(val, std::less {}, Proj);
};


export template <int N, int M>
constexpr auto matrix = std::views::adjacent_transform<N>([](auto&&... rows) {
	static_assert(std::ranges::range<decltype((rows, ...))>, "Input must be at least two-dimensional.");

	// TODO avoid copying into arrays
	auto as_array = [](auto&&... args) {
		return std::array { args... };
	};
	return std::views::zip_transform(as_array, (rows | std::views::adjacent_transform<M>(as_array))...);
}) | std::views::join;


// Produces a {value, coordinate} pair
export constexpr auto with_coord2d = std::views::enumerate | std::views::transform([](auto&& tup) {
	auto&& [y, row] = tup;

	auto zip_xform = [](int x, int y, auto&& val) {
		return std::pair{ val, kg::pos2di(x, y) };
		};

	return std::views::zip_transform(
		zip_xform,
		std::views::iota(0),
		std::views::repeat(y),
		row);
	})
	| std::views::join;

// Produces 2d coordinates without the value at the coordinate
export constexpr auto coord2d = std::views::enumerate | std::views::transform([](auto&& tup) {
	auto&& [y, row] = tup;

	auto zip_xform = [](int x, int y, auto&&) {
		return kg::pos2di(x, y);
		};

	return std::views::zip_transform(
		zip_xform,
		std::views::iota(0),
		std::views::repeat(y),
		row);
	})
	| std::views::join;

} // namespace kg::views
