#include <bit>
#include <iostream>
#include <string_view>

static constexpr std::string_view input{
#include "input.txt"
};

constexpr std::uint32_t to_bit(char ch) {
	return 1 << (ch - 'a');
}

template <std::size_t N>
constexpr std::size_t find_marker_start(std::string_view sv) {
	std::uint32_t mask = 0;
	for (std::size_t i = 0; i < N; i++)
		mask ^= to_bit(sv[i]);

	std::size_t i = N;
    while (std::popcount(mask) != N) {
        mask ^= to_bit(sv[i - N]);
        mask ^= to_bit(sv[i]);
		i += 1;
    }

	return i;
}

int main() {
	std::cout << find_marker_start<4>(input) << '\n';
	std::cout << find_marker_start<14>(input);
}
