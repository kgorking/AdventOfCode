#include <iostream>
#include <algorithm>
#include <sstream>
#include "input.h"

int wrapping_paper_needed(int length, int width, int height) {
	int const a1 = length * width;
	int const a2 = width * height;
	int const a3 = height * length;
	int const slack = std::min({ a1, a2, a3 });

	return 2*(a1 + a2 + a3) + slack;
}

int ribbon_needed(int length, int width, int height) {
	int const present = 2 * std::min({ length + width, width + height, height + length });
	int const bow = length * width * height;
	return present + bow;
}

int main() {
	std::istringstream ss{ input };

	int wrapping_paper = 0;
	int ribbon = 0;
	while (!ss.eof()) {
		int l = 0, w = 0, h = 0;
		ss >> l >> w >> h;

		wrapping_paper += wrapping_paper_needed(l, w, h);
		ribbon += ribbon_needed(l, w, h);
	}
	std::cout << "Square feet of wrapping paper needed: " << wrapping_paper << '\n';
	std::cout << "Feet of ribbon needed: " << ribbon << '\n';
}
