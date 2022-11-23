#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <stack>
#include <string_view>
#include <vector>

constexpr std::array<std::string_view, 110> input{
#include "input.txt"
};

bool is_open(char c) {
	switch (c) {
	case '(':
	case '{':
	case '[':
	case '<':
		return true;

	default:
		return false;
	}
}

bool is_close(char c) {
	switch (c) {
	case ')':
	case '}':
	case ']':
	case '>':
		return true;

	default:
		return false;
	}
}

bool match(char c1, char c2) {
	switch (c1) {
	case '(':
		return c2 == ')';
	case '{':
		return c2 == '}';
	case '[':
		return c2 == ']';
	case '<':
		return c2 == '>';

	default:
		return false;
	}
}

int get_score_p1(char c) {
	switch (c) {
	case ')':
		return 3;
	case ']':
		return 57;
	case '}':
		return 1197;
	case '>':
		return 25137;

	default:
		assert(false);
		return 0;
	}
}

int get_score_p2(char c) {
	switch (c) {
	case ')':
		return 1;
	case ']':
		return 2;
	case '}':
		return 3;
	case '>':
		return 4;

	default:
		assert(false);
		return 0;
	}
}

char get_matching_closing_bracket(char c) {
	switch (c) {
	case '(':
		return ')';
	case '{':
		return '}';
	case '[':
		return ']';
	case '<':
		return '>';

	default:
		assert(false);
		return 0;
	}
}

int main() {
	// Part 1
	int syntax_error_score = 0;

	std::vector<std::stack<char>> incomplete;

	for (std::string_view line : input) {
		std::stack<char> stack;
		bool corrupt = false;

		for (char c : line) {
			if (is_open(c))
				stack.push(c);
			else {
				if (!match(stack.top(), c)) {
					syntax_error_score += get_score_p1(c);
					corrupt = true;
					break;
				} else {
					stack.pop();
				}
			}
		}

		if (!corrupt && !stack.empty())
			incomplete.push_back(stack);
	}

	std::cout << "Part 1: " << syntax_error_score << '\n';

	// Part 2
	std::vector<int64_t> scores;
	for (auto& stack : incomplete) {
		int64_t score = 0;

		while (!stack.empty()) {
			score *= 5;
			score += get_score_p2(get_matching_closing_bracket(stack.top()));
			stack.pop();
		}

		scores.push_back(score);
	}

	auto const middle = scores.begin() + scores.size() / 2;
	std::ranges::nth_element(scores, middle);
	std::cout << "Part 2: " << *middle << '\n';
}
