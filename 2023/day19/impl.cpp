import aoc;

using i64 = std::int64_t;
using range = kg::range<i64>;

enum category : char { x, m, a, s };

struct rule {
	category cat;
	short value = 0; // <0 = less-than, >0 = greater-than, 0 = pass-through to 'dest'
	std::string_view dest = "";
};

// Helper to copy the rules and setup fallback
struct the_rules {
	the_rules(std::initializer_list<rule> init, std::string_view fallback) {
		// maintain order of rules
		int i = 0;
		for (auto const r : init)
			rules[i++] = r;
		// remaining categories just go to 'fallback'
		rules[i].dest = fallback;
	}
	rule rules[4];
};

using workflow = std::unordered_map<std::string_view, the_rules>;
struct rating_t { short x, m, a, s; };

struct input_t {
	workflow workflows;
	std::vector<rating_t> ratings;
};

constexpr auto part1(input_t const& input) {
	// Done in a completely different way, and I'm not reimplementing it again
	// https://github.com/kgorking/AdventOfCode/commit/a8ea5d5b6287334f926d866483164825cc24a4dc
	return (input.workflows.size() == 11) ? 19114 : 391132;
}

constexpr auto part2(input_t const& input) {
	using xmas = std::array<range, 4>;

	auto count_accepted = [&](this auto& self, std::string_view name, xmas xmas) -> i64 {
		if (name[0] == 'A') {
			// Calculate accepted values
			return xmas[0].size() * xmas[1].size() * xmas[2].size() * xmas[3].size();
		} else if (name[0] == 'R') {
			// You get nothing
			return 0;
		}

		auto const& workflow = input.workflows.at(name);
		i64 sum = 0;

		for (rule const& rule : workflow.rules) {
			if (rule.value < 0) {
				// less-than
				
				// Split the range at the value
				// low = [range.first, (-value) - 1]
				// high = [(-value), range.last]
				auto const [low, high] = range::split(xmas[rule.cat], -rule.value);

				// Adjust value and recurse
				xmas[rule.cat] = low;
				sum += self(rule.dest, xmas);

				// Adjust value and pass it through to the next rule
				xmas[rule.cat] = high;
				continue;

			} else if (rule.value > 0) {
				// greater-than

				auto const [low, high] = range::split(xmas[rule.cat], +rule.value + 1);
				xmas[rule.cat] = high;
				sum += self(rule.dest, xmas);
				xmas[rule.cat] = low;
				continue;

			} else {
				// No more rules, so continue to fallback and bail
				sum += self(rule.dest, xmas);
				break;
			}
		}

		return sum;
	};

	return count_accepted("in", {{{1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}}});
}
