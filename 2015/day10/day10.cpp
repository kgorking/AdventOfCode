#include <iostream>
#include <string>

std::string look_and_say(std::string const& input) {
	char count = 1;
	char last_c = input[0];

	std::string result;
	for (size_t i = 1; i < input.size(); i++) {
		if (input[i] == last_c) {
			count++;
		}
		else {
			result += '0' + count;
			result += last_c;

			last_c = input[i];
			count = 1;
		}
	}

	result += '0' + count;
	result += last_c;
	return result;
}

int main() {
	std::string input{ "1113122113" };
	for (int i = 0; i < 50; i++)
		input = look_and_say(input);

	std::cout << input << ": " << input.size() << '\n';
}
