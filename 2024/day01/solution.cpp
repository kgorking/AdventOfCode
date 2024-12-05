export module aoc.day01;
import aoc;
import std;

export constexpr auto expected_sample = std::make_pair(11, 31);
export constexpr auto expected_input = std::make_pair(1320851, 26859182);

// input = std::array<std::pair<int,int>>

struct processed {
	std::vector<int> left, right;
};

processed process_input(auto&& input) {
	processed p;

	for (auto const [l, r] : input) {
		p.left.push_back(l);
		p.right.push_back(r);
	}
	std::ranges::sort(p.left);
	std::ranges::sort(p.right);

	return p;
}

// Optimized find function. Doesn't need to run to the end of range
// to make sure the value is found.
auto find_in_sorted (auto& i, auto const end, int const value) {
	while (i != end && *i < value)
		++i;
	return (*i == value) ? i : end;
}

// Count the repeats of 'value'. Moves 'i' past 'value'.
int count_repeats(auto& i, auto const end, int const value) {
	int count = 1;
	while (++i != end && *i == value)
		count += 1;
	return count;
}

export auto part1(auto&& input) {
	processed const p = process_input(input);
	return kg::sum(kg::views::zip_diff(p.left, p.right));
}


export auto part2(auto&& input) {
	processed const p = process_input(input);

	int similarity_score = 0;
	auto it_left = p.left.begin();
	auto it_right = p.right.begin();

	// Linearly walk the the two lists and calc the similarity score. O(N).
	//   similarity = value * count(value, left) * count(value, right)
	while (it_left != p.left.end()) {
		int const value = *it_left;

		// Count the number of times the value appears in the left list
		int const count_left = count_repeats(it_left, p.left.end(), value);

		// Find the value in the right list
		auto it_next_right = find_in_sorted(it_right, p.right.end(), value);
		if (it_next_right != p.right.end()) {
			// Count the number of times the value appears in the right list
			int const count_right = count_repeats(it_next_right, p.right.end(), value);

			// Calculate the similarity and add it to the score
			int const similarity = value * count_left * count_right;
			similarity_score += similarity;

			// Move right list iterator to next value
			it_right = it_next_right;
		}
	}

	return similarity_score;
}
