export module aoc:array;
import std;
import :pos2d;

export namespace kg {

// Returns true if the position 'i' is in bounds
constexpr bool is_valid(auto&& array, kg::pos2di i) {
	return (i.x >= 0 && i.y >= 0 && i.y < array.size() && i.x < array[0].size());
}

// If the position 'i' is in bounds, return the value at that position, otherwise 'oob_value'
constexpr auto at(auto&& array, kg::pos2di i, auto oob_value) {
	if (i.x >= 0 && i.y >= 0 && i.y < array.size() && i.x < array[0].size())
		return array[i.y][i.x];
	else
		return oob_value;
}

template <int N>
	requires(N > 0)
constexpr auto array_of(auto fill_value) {
	std::array<decltype(fill_value), N> arr;
	arr.fill(fill_value);
	return arr;
}
}
