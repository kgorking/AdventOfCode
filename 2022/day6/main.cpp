﻿#include <array>
#include <iostream>
#include <string_view>
#include <algorithm>
#include <bit>

static constexpr std::string_view input{
	#include "input.txt"
};

template<std::size_t N>
bool is_unique(std::string_view marker) noexcept {
	uint32_t bits = 0;
	for(char c : marker)
		bits |= 1 << (c - 'a');
	return N == std::popcount(bits);
}

template<std::size_t N>
std::size_t find_marker_start(std::string_view sv) {
	std::size_t processed = N;

	while(!is_unique<N>(sv.substr(processed - N, N))) {
		processed += 1;
	}

	return processed;
}

int main() {
	// Part 1
	std::cout << find_marker_start<4>(input) << '\n';

	// Part 2
	std::cout << find_marker_start<14>(input) << '\n';
}
