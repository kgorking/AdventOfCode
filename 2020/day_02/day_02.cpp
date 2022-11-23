#include <cstdio>
#include <iostream>
#include <algorithm>

int main() {
	FILE* file = nullptr;
	fopen_s(&file, "../day_02/input.txt", "r");

	int min = 0, max = 0;
	char c = 0;
	char string[64] = "";

	int valid_pws_1 = 0;
	int valid_pws_2 = 0;
	while (4 == fscanf_s(file, "%d-%d %c : %63s", &min, &max, &c, 1, &string, 64)) {
		auto const num1 = std::count_if(string, string + strlen(string) + 1, [c](auto in) { return c == in; });
		if (num1 >= min && num1 <= max)
			valid_pws_1++;

		auto const num2 = (string[min - 1] == c) + (string[max - 1] == c);
		if (num2 == 1)
			valid_pws_2++;
	}
	if (file)
		fclose(file);

	std::cout << valid_pws_1 << '\n';
	std::cout << valid_pws_2;
}
