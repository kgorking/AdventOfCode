#include <iostream>
#include <array>

enum class opcode {
	hlf,
	tpl,
	inc,
	jmp,
	jie,
	jio
};
enum /*class*/ reg {
	a,
	b,
	none,
};
using instruction = std::tuple<opcode, reg, int>;

std::array<instruction, 46> constexpr program{ instruction
	{opcode::jio, reg::a, 16},
	{opcode::inc, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::jmp, reg::none, +23},
	{opcode::tpl, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::tpl, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::jio, reg::a, +8},
	{opcode::inc, reg::b, 0},
	{opcode::jie, reg::a, +4},
	{opcode::tpl, reg::a, 0},
	{opcode::inc, reg::a, 0},
	{opcode::jmp, reg::none, +2},
	{opcode::hlf, reg::a, 0},
	{opcode::jmp, reg::none, -7}
};

int main() {
	size_t ip = 0;
	unsigned registers[2] { 1, 0 };

	while (ip < program.size()) {
		auto const [op, reg, offset] = program[ip];
		switch (op) {
			case opcode::hlf:
				registers[reg] >>= 1;
				ip++;
				break;
			case opcode::tpl:
				registers[reg] *= 3;
				ip++;
				break;
			case opcode::inc:
				registers[reg]++;
				ip++;
				break;
			case opcode::jmp:
				ip += offset;
				break;
			case opcode::jie:
				ip += ((registers[reg] & 1) == 0) ? offset : 1;
				break;
			case opcode::jio:
				ip += (registers[reg] == 1) ? offset : 1;
				break;
			default: throw;
		}
	}

	std::cout << "b: " << registers[reg::b] << '\n';
}
