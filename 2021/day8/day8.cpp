#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <iostream>
#include <string_view>

struct entry {
	std::array<std::string_view, 10> signals;
	std::array<std::string_view, 4> digits;
};
constexpr std::array<entry, 200> input{{
#include "input.txt"
}};

// gfedcba
using display = std::bitset<7>;
constexpr std::array<display, 10> normal_digits = {
	0b1110111, // 0 - 6 bits - abc efg
	0b0100100, // 1 - 2 bits -   c  f
	0b1011101, // 2 - 5 bits - a cde g
	0b1101101, // 3 - 5 bits - a cd fg
	0b0101110, // 4 - 4 bits -  bcd f
	0b1101011, // 5 - 5 bits - ab d fg
	0b1111011, // 6 - 6 bits - ab defg
	0b0100101, // 7 - 3 bits - a c  f
	0b1111111, // 8 - 7 bits - abcdefg
	0b1101111, // 9 - 6 bits - abcd fg
};

// Convert from text to bits
display to_bitset(std::string_view sv) {
	display d;
	for (size_t c : sv)
		d |= 1ll << (c - 'a');
	return d;
}

// Convert from text to a value using translator
ptrdiff_t to_value(std::string_view sv, std::array<display, 10> const& digits) {
	display const d = to_bitset(sv);
	auto const it = std::ranges::find(digits, d);
	assert(it != digits.end());
	return std::distance(digits.begin(), it);
}

void part1() {
	int count = 0;
	for (entry const& e : input) {
		for (std::string_view sv : e.digits) {
			switch (sv.size()) {
			case 2:
			case 3:
			case 4:
			case 7:
				count += 1;
				break;

			default:
				break;
			}
		}
	}

	std::cout << "Part 1: " << count << '\n';
}

void part2() {
	ptrdiff_t sum = 0;

	for (entry const& e : input) {
		// Translation segments
		std::array<display, 10> digits;
		std::array<display, 7> translation;
		digits.fill(0b1111111);
		translation.fill(0b1111111);

		// Find unique digits
		for (std::string_view sig : e.signals) {
			auto const bits = to_bitset(sig);

			switch (sig.size()) {
			case 2:
				digits[1] = bits;
				break;

			case 3:
				digits[7] = bits;
				break;

			case 4:
				digits[4] = bits;
				break;

			case 5:
			case 6:
			case 7:
				break;

			default:
				assert(false);
				break;
			}
		}

		// Initial translation setup
		translation[0] = digits[7] & ~digits[1]; // a holds 'a'
		translation[1] = digits[4] & ~digits[7]; // b holds 'bd'
		translation[2] = digits[1];				 // c holds 'cf'
		translation[3] = translation[1];		 // d holds 'bd'
		translation[4] &= ~translation[0];		 //
		translation[4] &= ~translation[1];		 //
		translation[4] &= ~translation[2];		 // e holds 'eg'
		translation[5] = translation[2];		 // f holds 'cf'
		translation[6] = translation[4];		 // g holds 'eg'

		// Deduce remaining segments (bits)
		for (std::string_view sig : e.signals) {
			auto const bits = to_bitset(sig);

			switch (sig.size()) {
			case 5:						// 'adg', ~'bcef'
				translation[3] &= bits; // keep 'adg' in d
				break;

			case 6:						// 'abfg', ~'cde'
				translation[1] &= bits; // keep 'abfg' in b
				// translation[2] &= ~bits;	// keep 'cde' in c
				translation[5] &= bits; // keep 'abfg' in f
				translation[6] &= bits; // keep 'abfg' in g
				break;

			case 7:
				break;

			default:
				break;
			}
		}
		translation[2] &= ~translation[5]; // remove f from 'cf'
		translation[4] &= ~translation[6]; // remove g from 'eg'

		// Reconstruct digits
		digits[0] &= ~translation[3]; // remove 'd' from 0

		digits[2] &= ~translation[1]; // remove 'b' from 2
		digits[2] &= ~translation[5]; // remove 'f' from 2

		digits[3] &= ~translation[1]; // remove 'b' from 3
		digits[3] &= ~translation[4]; // remove 'e' from 3

		digits[5] &= ~translation[2]; // remove 'c' from 5
		digits[5] &= ~translation[4]; // remove 'e' from 5

		digits[6] &= ~translation[2]; // remove 'c' from 6

		digits[9] &= ~translation[4]; // remove 'e' from 9

		auto test = digits;
		std::ranges::sort(test, std::ranges::less{}, &display::to_ulong);
		auto const end = std::ranges::unique(test);
		assert(end.empty());

		// Calculate the sum of the shown digits
		auto const val = 1000 * to_value(e.digits[0], digits) + 100 * to_value(e.digits[1], digits) + 10 * to_value(e.digits[2], digits) +
						 1 * to_value(e.digits[3], digits);

		sum += val;
	}

	std::cout << "Part 2: " << sum << '\n';
}

int main() {
	part1();
	part2();
}
