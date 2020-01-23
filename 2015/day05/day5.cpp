#include <iostream>
#include "input.h"

//#define PART1

#ifdef PART1
bool is_vowel(char c) {
	std::array<char, 5> constexpr vowels{ 'a','e','i','o','u' };
	return std::find(vowels.begin(), vowels.end(), c) != vowels.end();
}

bool is_double_letters(char l, char r) {
	// xx, ff, etc.
	return l == r;
}

bool is_bad_string(char l, char r) {
	// ab, cd, pq, xy
	return
		(l == 'a' && r == 'b') ||
		(l == 'c' && r == 'd') ||
		(l == 'p' && r == 'q') ||
		(l == 'x' && r == 'y');
}

int main() {
	int nice_strings = 0;

	//std::array<std::array<char, 17>, 1> constexpr input{ { "jchzalrnumimnmhp" } };

	for (auto const& string : input) {
		int vowels = 0;
		int double_letters = 0;
		int bad_strings = 0;

		for (int i = 0; i < 15; i++) {
			vowels += is_vowel(string[i]);

			double_letters += is_double_letters(string[i], string[i + 1]);

			bad_strings += is_bad_string(string[i], string[i + 1]);
		}
		vowels += is_vowel(string[15]);

		if (bad_strings == 0 && double_letters > 0 && vowels >= 3)
			nice_strings++;
	}

	std::cout << "Nice strings: " << nice_strings << '\n';
}
#else // PART 2
#include <set>
int main() {
	using pair = std::pair<char, char>;

	//std::array<std::array<char, 17>, 1> constexpr input{ { "qjhvhtzxzqqjkmpb" } }; // nice
	//std::array<std::array<char, 17>, 1> constexpr input{ { "qqqvhtzxzqdjkmpb" } }; // naughty
	//std::array<std::array<char, 17>, 1> constexpr input{ { "uurcxstgmygtbstg" } }; // naughty
	//std::array<std::array<char, 17>, 1> constexpr input{ { "ieodomkazucvgmuy" } }; // naughty

	int nice_strings = 0;
	for (auto const& string : input) {
		std::set<pair> all_pairs;
		int num_pairs_found = 0;
		int num_repeats = 0;

		for (int i = 0; i < 14; i += 2) {
			num_repeats += (string[i + 0] == string[i + 2]);
			num_repeats += (string[i + 1] == string[i + 3]);

			pair const p1{ string[i + 0], string[i + 1] };
			pair const p2{ string[i + 1], string[i + 2] };

			if (all_pairs.count(p1) > 0) num_pairs_found++;
			if (all_pairs.count(p2) > 0) num_pairs_found++;

			all_pairs.insert(p1);
			all_pairs.insert(p2);
		}

		if (num_pairs_found > 0 && num_repeats > 0)
			nice_strings++;

		//std::cout << "Pairs:   " << num_pairs_found << '\n';
		//std::cout << "Repeats: " << num_repeats << '\n';
	}

	std::cout << "\nNice strings: " << nice_strings << '\n';
}
#endif
