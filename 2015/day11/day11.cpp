#include <iostream>
#include <array>

using password = std::array<char, 8>;

bool has_increasing_straight(password const& pw) {
	for (size_t i = 0; i < pw.size() - 2; i++) {
		if ((pw[i] == pw[i + 1] - 1) &&
			(pw[i] == pw[i + 2] - 2))
			return true;
	}

	return false;
}

bool has_valid_letters(password const& pw) {
	auto const is_valid_letter = [](char c) {
		return
			c != 'i' &&
			c != 'o' &&
			c != 'l';
	};

	return std::all_of(pw.begin(), pw.end(), is_valid_letter);
}

bool has_letter_pairs(password const& pw) {
	char first_letter_pair = 0;
	for (size_t i = 0; i < pw.size() - 1; i++) {
		if (pw[i] == pw[i + 1] && pw[i] != first_letter_pair) {
			if (first_letter_pair != 0)
				return true;
			else
				first_letter_pair = pw[i];
		}
	}

	return false;
}

bool is_valid(password const& pw) {
	return
		has_increasing_straight(pw) &&
		has_valid_letters(pw) &&
		has_letter_pairs(pw);
}

void inc_helper(password& pw, int index) {
	if (pw[index] == 'z') {
		pw[index] = 'a';
		inc_helper(pw, index - 1);
	}
	else {
		pw[index]++;
	}
}

void increment(password& pw) {
	inc_helper(pw, 7);
}

void print(password const& pw) {
	for (char c : pw)
		std::cout << c;
	std::cout << '\n';
}

void find_next(password& pw) {
	do {
		increment(pw);
	} while (!is_valid(pw));
}

int main() {
	password pw{ 'h', 'x', 'b', 'x', 'w', 'x', 'b', 'a' };

	find_next(pw);
	print(pw);

	find_next(pw);
	print(pw);
}
