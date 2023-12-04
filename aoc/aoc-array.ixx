export module aoc:array;
import std;

export namespace kg {
template <int N>
	requires(N > 0)
auto array_of(auto fill_value) {
	std::array<decltype(fill_value), N> arr;
	arr.fill(fill_value);
	return arr;
}
}
