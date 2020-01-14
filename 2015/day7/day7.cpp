#include <iostream>
#include <map>
#include <vector>
#include <variant>
#include <list>
#include <optional>
#include <charconv>
#include "input.h"

using signal = unsigned short;
using identifier = std::string_view;

enum class gate_op {
	And,
	Or,
	LShift,
	RShift,
	Not,
};

struct wire;
struct gate {
	std::variant<wire const*, signal> const input1;
	std::variant<wire const*, signal> const input2;
	gate_op const op;

	mutable std::optional<signal> cache;
};

struct wire {
	//           node,  node,  leaf
	std::variant<gate const*, wire const*, signal> value;
};


// Forward decl
signal run(wire const& w);

signal run(gate const& g) {
	if (g.cache)
		return *g.cache;

	signal const s1 = (g.input1.index() == 0) ? run(*std::get<wire const*>(g.input1)) : std::get<signal>(g.input1);

	signal val = 0;
	switch (g.op) {
		case gate_op::And:      val = s1 & run(*std::get<wire const*>(g.input2)); break;
		case gate_op::Or:       val = s1 | run(*std::get<wire const*>(g.input2)); break;
		case gate_op::LShift:   val = s1 << std::get<signal>(g.input2); break;
		case gate_op::RShift:   val = s1 >> std::get<signal>(g.input2); break;
		case gate_op::Not:      val = ~s1; break;
	}

	g.cache = val;
	return val;;
}

signal run(wire const& w) {
	switch (w.value.index()) {
		case 0: return run(*std::get<gate const*>(w.value));
		case 1: return run(*std::get<wire const*>(w.value));
		case 2: return std::get<signal>(w.value);
	}

	throw;
}

// Calls the function f on each part of the string divided by the 'splitter' character
template <typename Fn>
void for_each_part(std::string_view s, char splitter, Fn f) {
	int offset = 0;
	int nl = s.find_first_of(splitter, 0);
	while (nl != std::string_view::npos) {
		std::string_view part = s.substr(offset, nl - offset);

		f(part);

		offset = nl + 1;
		nl = s.find_first_of(splitter, offset);
	}

	f(s.substr(offset));
}

// Converts an operator name to its enum value
gate_op get_operand_from_string(std::string_view s) {
	switch (s[0]) {
		case 'A': return gate_op::And;
		case 'O': return gate_op::Or;
		case 'L': return gate_op::LShift;
		case 'R': return gate_op::RShift;
		case 'N': return gate_op::Not;
	}

	throw;
}

// string-to-signal conversion
signal to_signal(std::string_view s) {
	unsigned long val = 0;
	if (std::errc::invalid_argument == std::from_chars(s.data(), s.data() + s.size(), val).ec) {
		std::cout << "invalid integer value: " << s << '\n';
		throw;
	}

	if (val > std::numeric_limits<signal>::max()) {
		std::cout << "integer to large: " << s << '\n';
		throw;
	}

	return static_cast<signal>(val);
}


int main() {
	std::map<identifier, wire> wire_map;
	std::list<gate> gates;

	// Parse the input on a per-line basis
	for_each_part(input, '\n', [&](auto line) {
		if (line.empty())
			return;

		// Split the line into its parts
		std::vector<std::string_view> parts;
		for_each_part(line, ' ', [&parts](auto part) {
			parts.push_back(part);
		});

		// Parse the parts
		switch (parts.size()) {
			case 3: {
				char const first_char = parts[0].at(0);

				if (isdigit(first_char)) {
					// 123 -> x
					wire_map[parts[2]].value = to_signal(parts[0]);
				}
				else {
					// x -> y
					wire_map[parts[2]].value = &wire_map[parts[0]];
				}

				break;
			}

			case 4: {
				// NOT x -> f
				gates.push_back({ &wire_map[parts[1]], nullptr, gate_op::Not });
				wire_map[parts[3]].value = &gates.back();
				break;
			}

			case 5: {
				// "x OP y -> z"
				auto const parse_wire = [&wire_map](auto sv) {
					return isdigit(sv[0]) ?
						std::variant<wire const*, signal>{ to_signal(sv) } :
						std::variant<wire const*, signal>{ &wire_map[sv] };
				};

				gates.push_back({
					parse_wire(parts[0]),
					parse_wire(parts[2]),
					get_operand_from_string(parts[1]) });

				wire& out = wire_map[parts[4]];
				out.value = &gates.back();

				break;
			}

			default:
				throw;
		}
	});

	signal const sig_a = run(wire_map["a"]);
	std::cout << "Wire a: " << sig_a << '\n';

	// a -> b
	auto& wire_b = wire_map["b"];
	wire_b.value = sig_a;

	// Reset all the caches
	for (auto& gate : gates) {
		gate.cache.reset();
	}

	signal const new_sig_a = run(wire_map["a"]);
	std::cout << "New wire a: " << new_sig_a << '\n';
}
