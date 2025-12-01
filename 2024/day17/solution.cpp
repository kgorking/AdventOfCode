export module aoc.day17;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(0, 0);
export constexpr auto expected_input = std::make_pair(0, 0);

void computer(int A, int B, int C, std::ranges::range auto&& program, auto&& fn_out) {
	auto combo = [&](int v) {
		switch (v) {
		case 0:
		case 1:
		case 2:
		case 3: return v;
		case 4: return A;
		case 5: return B;
		case 6: return C;
		default: std::abort();
		}
	};

	int ip = 0;
	while (ip < program.size()) {
		int const op = program[ip + 0];
		int const operand = program[ip+1];
		switch(op) {
		/* adv */ case 0: A = A / std::pow(2, combo(operand)); break;
		/* bxl */ case 1: B = B ^ operand; break;
		/* bst */ case 2: B = combo(operand) % 8; break;
		/* jnz */ case 3: if (A != 0) { ip = operand; continue; } break;
		/* bxc */ case 4: B = B ^ C; break;
		/* out */ case 5: if(!fn_out(combo(operand) % 8)) return; break;
		/* bdv */ case 6: B = A / std::pow(2, combo(operand)); break;
		/* cdv */ case 7: C = A / std::pow(2, combo(operand)); break;
		}

		ip += 2;
	}
}

export auto part1(auto&& input) {
	auto [A, B, C, program] = input;
	std::print("Part 1: ");
	computer(A, B, C, program, [](int out) {
		std::cout << out << ',';
		return true;
	});
	std::println();

	return 0;
}

export auto part2(auto&& input) {
	// decompiled
	/*
	* B = A % 8
	* B = B ^ 5
	* C = A / 2^B  // A >> B
	* B = B ^ 6
	* A = A / 8
	* B = B ^ C
	* write B
	* jnz A 0
	* 
	* 
	* B = (A % 8) ^ 5
	* C = A >> B
	* B = (B ^ 6) ^ C
	* write B
	* A = A / 8    // A >> 3
	* jnz A 0
	*/
	// The final character in the program only depends on the final 3 bits of A. Work backwards from that.

	std::print("Part 2: ");

	auto [xA, xB, xC, program] = input;

	// Generate lookup table
	int found_A = 0;
	for (int i = 0; i < program.size(); i++) {
		computer(i, 0, 0, program, [&](int out) {
			if (out == program.front()) {
				std::cout << out << ',';
				found_A <<= 3;
				found_A |= i;
			}
			return true;
		});
	}
	std::println();

	// Generate A
	//for (int c : program) {
	//	found_A |= lookup[c];
	//	found_A <<= 3;
	//}

	std::print("A={}, ", found_A);
	computer(found_A, 0, 0, program, [&](int out) {
		std::cout << out << ',';
		return true;
		});
	std::println();

	return 0;
}
