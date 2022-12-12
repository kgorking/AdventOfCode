// https://adventofcode.com/2022/day/9
#include <array>
#include <iostream>
#include <queue>
#include <string_view>

enum code : char { noop = 1, addx };
struct instruction {
	code c;
	char val = 0;
};

static constexpr auto input = std::to_array<instruction>({
#include "input.txt"
});

int run_program(char* crt) {
	int X = 1;
	std::queue<char> pipeline;

	int checksum = 0;
	int cycle = 1;

	// Runs a single cycle
	const auto run_cycle = [&]() {
		// Draw to the crt
		int const x_pos = (cycle - 1) % 40;
		*crt++ = '.' - 11 * (x_pos >= X - 1) * (x_pos <= X + 1);

		// update register
		if (!pipeline.empty()) {
			X += pipeline.front();
			pipeline.pop();
		}

		cycle += 1;

		bool const check = (cycle - 20) % 40 == 0;
		if (check && cycle <= 220) {
			checksum += cycle * X;
		}
	};

	for (instruction const i : input) {
		// load next instruction in the pipeline
		pipeline.push(0);
		if (i.c == addx)
			pipeline.push(i.val);

		run_cycle();
	}

	// flush rest of the pipeline
	while (!pipeline.empty()) {
		run_cycle();
	}

	return checksum;
}

int main() {
	std::array<char, 40 * 6> CRT;
	CRT.fill('X');
	int const part1 = run_program(CRT.data());
	std::cout << "Part 1: " << part1 << '\n' << '\n';

	std::cout << "Part 2: " << '\n';
	std::string_view const sv(CRT);
	for (int i = 0; i < 6; i++) {
		std::cout << sv.substr(i * 40, 40) << '\n';
	}
}
