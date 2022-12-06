#include <array>
#include <iostream>
#include <string_view>
#include <stack>
#include <queue>

struct instruction {
	char move, from, to;
};

struct input_t {
	std::array<std::string_view, 9> stacks;
	std::array<instruction, 501> instructions;
};

static constexpr input_t input{
	#include "input.txt"
};

using cratestack = std::stack<char, std::string>;

using cratemover9000 = std::queue<char>;
using cratemover9001 = std::stack<char>;

constexpr auto init_stacks() {
	std::array<cratestack, 9> stacks;
	for(int i=0; i<9; i++)
		stacks[i] = cratestack(input.stacks[i].begin(), input.stacks[i].end());
	return stacks;
}

int main() {
	auto stacks = init_stacks();

	// Part 1 - use a queue
	cratemover9000 cm9000;
	for(instruction const& inst : input.instructions) {
		for(char i = 0; i<inst.move; i++) {
			cm9000.push(stacks[inst.from-1].top());
			stacks[inst.from-1].pop();
		}

		for(char i = 0; i<inst.move; i++) {
			stacks[inst.to-1].push(cm9000.front());
			cm9000.pop();
		}
	}

	// Print the top crates
	for (auto const& stack : stacks)
		std::cout << stack.top();
	std::cout << '\n';

	// Part 2 - use a stack
	stacks = init_stacks();
	cratemover9001 cm9001;
	for(instruction const& inst : input.instructions) {
		for(char i = 0; i<inst.move; i++) {
			cm9001.push(stacks[inst.from-1].top());
			stacks[inst.from-1].pop();
		}

		for(char i = 0; i<inst.move; i++) {
			stacks[inst.to-1].push(cm9001.top());
			cm9001.pop();
		}
	}

	// Print the top crates
	for (auto const& stack : stacks)
		std::cout << stack.top();
	std::cout << '\n';
}
