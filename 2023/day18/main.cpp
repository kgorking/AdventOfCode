import aoc;

constexpr auto input = std::to_array<std::string_view>({
#include "input.txt"
});

auto solve() {
	return std::make_pair(0, 0);
}

int main() {
	auto const [p1, p2] = solve();
	std::cout << "Part 1: " << p1 << '\n';
	std::cout << "Part 2: " << p2 << '\n';
}
