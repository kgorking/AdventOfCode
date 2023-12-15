import aoc;

struct input_t {
	std::string_view record;
	std::vector<char> groups;
};

void backtrack(std::set<std::string>& out, std::string record, std::vector<char> const& groups, int group_index, int i) {
	std::cout << "       " << record << '\n';

	auto unknown = kg::to_bitset<64>(record, '?');
	auto working = kg::to_bitset<64>(record, '#');

	kg::wildcard_pattern_matcher pm("?#?#?#?#?#?#?#?");
	std::cout <<         pm.matches(".#.###.#.######");
}

constexpr auto part1(auto const& input) {
	extern bool aoc_dev_mode;
	aoc_dev_mode = true;

	// prune
	// backtrack
	int sum = 0;
	//for (auto const& i : input)
	auto const& i = input[3];
	{
		std::string record = i.record.data();
		std::set<std::string> result;
		std::cout << "start: " << record << '\n';
		backtrack(result, record, i.groups, 0, 0);
		sum += result.size();

		std::cout << "      --------------------" << '\n';
		for (auto const& s : result)
			std::cout << "       " << s << '\n';
	}

	return sum;
}

constexpr auto part2(auto const& input) {
	return 0;
}

auto solve(auto const& input) {
	return std::make_pair(part1(input), part2(input));
}
