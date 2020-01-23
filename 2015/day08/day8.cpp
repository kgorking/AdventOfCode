#include <iostream>
#include "input.h"

// Calls the function f on each part of the string divided by the 'splitter' character
template <typename Fn>
void for_each_part(std::string_view s, char splitter, Fn func) {
	size_t offset = 0;
	size_t index = s.find_first_of(splitter, 0);
	while (index != std::string_view::npos) {
		func(s.substr(offset, index - offset));

		offset = index + 1;
		index = s.find_first_of(splitter, offset);
	}

	func(s.substr(offset));
}

int main() {
	int code_length = 0;
	int mem_length = 0;

	// \\ \" \x00
	for_each_part(input, '\n', [&](std::string_view  line) {
		for (int i = 0; i < line.length(); i++) {
			switch (line[i]) {
				case '\"':
					code_length++;
					break;

				case '\\': {
					switch (line[i + 1]) {
						case '\\':
						case '"':
							code_length += 2;
							mem_length += 1;
							i++;
							break;

						case 'x':
							code_length += 4;
							mem_length += 1;
							i += 3; // x88
							break;
					}

					break;
				}

				default: { // simple character
					code_length++;
					mem_length++;
					break;
				}
			}
		}
	});

	std::cout << code_length - mem_length << '\n';

	int encode_length = 0;
	for_each_part(input, '\n', [&](std::string_view  line) {
		encode_length += 2; // 2x \"

		for (int i = 0; i < line.length(); i++) {
			switch (line[i]) {
				case '"':
					encode_length += 2;
					break;

				case '\\':
					encode_length += 2;
					break;

				default: { // simple character
					encode_length++;
					break;
				}
			}
		}
	});

	std::cout << encode_length - code_length << '\n';
}

