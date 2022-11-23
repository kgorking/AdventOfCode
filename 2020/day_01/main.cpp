#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>

int main() {
	std::bitset<2020> bitset;

	std::ifstream f_input("../day_01/input.txt");
	int val = 0;
	while (f_input >> val) {
		bitset.set(val);
	}

	for (auto a = 1; a <= 2020; a++) {
		if (!bitset.test(a))
			continue;

		for (auto b = a + 1; (2020 - a - b) > 0 ; b++) {
			if (!bitset.test(b))
				continue;

			if (bitset.test(2020 - a - b)) {
				std::cout << "result : " << (a * b * (2020 - b - a));
				return 0;
			}
		}
	}
}
