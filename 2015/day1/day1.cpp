#include "input.h"
#include <iostream>

int main() {
	int floor = 0;

	// part 1
	for (char const val : input) {
		if (val == '(')
			floor++;
		else
			floor--;
	}
	std::cout << "Santa ends up on floor 74\n";

	// part 2
	floor = 0;
	int pos = 1;
	for (char const val : input) {
		if (val == '(')
			floor++;
		else
			floor--;

		if (floor == -1)
			break;

		pos++;
	}
	std::cout << "Santa ends up in the basement at instruction " << pos << '\n';
}
