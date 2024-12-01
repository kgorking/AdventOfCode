import aoc;
import std;

namespace AOC_NS {

constexpr auto expected_sample = std::make_pair(11, 31);
constexpr auto expected_input = std::make_pair(1320851, 26859182);
constexpr auto sample_input = get_sample_input();
constexpr auto actual_input = get_input();

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

// Count the repeats of 'value'. Moves 'i' past 'value'.
int count_repeats(auto& i, auto const end, int const value) {
	int count = 1;
	while (++i != end && *i == value)
		count += 1;
	return count;
}

auto part1(auto&& input) {
	processed const p = process_input(input);

	auto distance = [](int l, int r) -> int { return std::abs(l - r); };

	const auto distances = std::views::zip_transform(distance, p.left, p.right);
	return std::ranges::fold_left(distances, 0, std::plus{});
}


auto part2(auto&& input) {
	processed const p = process_input(input);

	int similarity_score = 0;
	auto it_left = p.left.begin();
	auto it_right = p.right.begin();

	// Linearly walk the the two lists and calc the similarity score.
	//   similarity = value * count(value, left) * count(value, right)
	while (it_left != p.left.end()) {
		int const value = *it_left;

		// Count the number of times the value appears in the left list
		int const count_left = count_repeats(it_left, p.left.end(), value);

		// Find the value in the right list
		auto it_next_right = std::ranges::find(it_right, p.right.end(), value);
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
} // namespace AOC_NS
