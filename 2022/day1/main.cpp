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

int main() {
	std::array<int, 3> result;
	std::ranges::partial_sort_copy(input | std::views::transform(reduce), result, std::greater{});

	// Part 1
	std::cout << std::ranges::max(result) << '\n';

	// Part 2
	std::cout << reduce(result) << '\n';
}
