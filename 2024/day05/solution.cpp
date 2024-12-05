export module aoc.day05;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(143, 123);
export constexpr auto expected_input = std::make_pair(4662, 5900);

auto solution(auto&& input, bool fix) {
	auto [rules, all_updates] = input;

	std::multimap<int, int> ruleset;
	for (auto [before, after] : rules) {
		ruleset.insert({ after, before }); // NOTE: switched
	}

	// Accumulate the valid/fixed values
	int num_valid[2] = { 0, 0 };
	bool value_fixed = false;

	for (std::vector<int>& updates : all_updates) {
		value_fixed = false;
		int count_invalid = 0;
		int mid_value = updates[updates.size() / 2];

		do {
			// Check how many invalid pages are present
			std::map<int, int> checks;
			for (int i = 0; i < updates.size(); i++) {
				// Set the current pages count to the number of pages after it
				int const page = updates[i];
				checks[page] = updates.size() - i - 1;

				// Decrement former pages counts if they need to be located before the current page
				for (auto [i, end] = ruleset.equal_range(page); i != end; i++) {
					if (checks.contains(i->second))
						checks[i->second] -= 1;
				}
			}

			count_invalid = kg::sum(checks | std::views::values);
			if (count_invalid > 0) {
				if (!fix)
					mid_value = 0;
				else {
					value_fixed = true;
					for (auto [page, bad_count] : checks) {
						if (0 == bad_count)
							continue;
						// The remaining count for the bad page is
						// used to move it forward in the list of pages.
						auto it = std::ranges::find(updates, page);
						std::ranges::swap(*it, *(it + bad_count));

						// Update the mid-value, as it might have changed
						mid_value = updates[updates.size() / 2];

						// Only fix one page at the time
						break;
					}
				}
			}
		} while (count_invalid && fix);

		num_valid[value_fixed] += mid_value;
	}

	return num_valid[fix];
}

export auto part1(auto&& input) { return solution(input, false); }
export auto part2(auto&& input) { return solution(input, true); }
