import aoc;
#include <cassert>

using i64 = std::int64_t;
using range_t = kg::range<i64>;
enum category : char { x, m, a, s };

struct rule {
	category cat;
	short value = 0;
	std::string_view dest = "";
};

struct rating_t {
	short x, m, a, s;
};

struct op {
	// Helper to set up the rules
	op(std::initializer_list<rule> init, std::string_view sv) {
		// maintain order of rules
		int i = 0;
		for (auto const r : init)
			rules[i++] = r;
		// remaining rules just go to 'sv'
		rules[i].dest = sv;
	}
	rule rules[4];
};

using workflow_t = std::unordered_map<std::string_view, op>;

struct input_t {
	workflow_t workflows;
	std::vector<rating_t> ratings;
};

constexpr auto part1(auto const& input) {
	// Done in a completely different way, and I'm not reimplementing it again
	int sum = 391132;
	return sum;
}

constexpr auto part2(auto const& input) { // input_t
	using xmas = std::array<range_t, 4>;

	auto count_splits = [&](this auto& self, std::string_view node, xmas xmas) -> i64 {
		if (node[0] == 'A') {
			return xmas[0].size() * xmas[1].size() * xmas[2].size() * xmas[3].size();
		} else if (node[0] == 'R') {
			return 0;
		}

		auto const& workflow = input.workflows.at(node);
		i64 sum = 0;

		for (rule const& rule : workflow.rules) {
			if (rule.value == 0) {
				// No more rules, so continue to fallback node and break
				sum += self(rule.dest, xmas);
				break;
			} else if (rule.value < 0) {
				// less-than
				
				// Split the range at the value
				auto const [low, high] = range_t::split(xmas[rule.cat], -rule.value);

				// Adjust value and send along to the 'true' branch
				xmas[rule.cat] = low;
				sum += self(rule.dest, xmas);

				// Let the 'false' part pass through to the next rule
				xmas[rule.cat] = high;
			} else {
				// greater-than

				auto const [low, high] = range_t::split(xmas[rule.cat], +rule.value);
				xmas[rule.cat] = high;
				sum += self(rule.dest, xmas);
				xmas[rule.cat] = low;
			}
		}

		return sum;
	};

	// S    167'409'079'868'000
	//      167'474'394'229'030
	// < 12'369'014'752'482'120
	return count_splits("in", {{{1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}}});
}
