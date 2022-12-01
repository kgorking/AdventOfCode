#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <ranges>

const std::vector<std::vector<int>> input{
	#include "input.txt"
};

constexpr auto reduce = [](auto const& vec) { return std::reduce(vec.begin(), vec.end()); };
constexpr auto max = [](int l, int r){ return std::max(l,r); };

int main() {
	// Part 1
	std::cout << std::transform_reduce(input.begin(), input.end(), 0, max, reduce) << '\n';

	// Part 2
	std::array<int, 3> result;
	std::ranges::partial_sort_copy(input | std::views::transform(reduce), result, std::greater{});
	std::cout << reduce(result);
}
