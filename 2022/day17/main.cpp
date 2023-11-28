import aoc;

constexpr auto input = std::to_array<std::string_view>({
#include "input.txt"
});

int main() {
	std::bitset<16> bs(0b1000110101010001);
	std::cout << "Part 0: " << bs.to_ulong() << '\n';

	auto subset = bs;
	for (int i = 0; i < 64; i++)
		subset = kg::bit_subtract(subset, {1}) & bs;

	// Part 1
	std::cout << "Part 1: " << subset.to_ulong() << '\n';

	// Part 2
	subset = kg::bit_subtract(bs, {64}) & bs;
	std::cout << "Part 2: " << subset.to_ulong() << '\n';
}
