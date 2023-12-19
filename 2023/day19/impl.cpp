import aoc;

#define RULE +[](int x, int m, int a, int s) -> std::string_view 

using workflow_t = std::unordered_map<std::string_view, std::string_view(*)(int,int,int,int)>;
struct rating_t { int x, m, a, s; };

struct input_t {
	workflow_t workflows;
	std::vector<rating_t> ratings;
};

constexpr auto part1(auto const& input) {
	int sum = 0;
	for (auto const [x, m, a, s] : input.ratings) {
		std::string_view name = "in";
		while (true) {
			name = input.workflows.at(name)(x, m, a, s);

			if (name[0] == 'A') {
				sum += (x + m + a + s);
				break;
			} else if (name[0] == 'R') {
				break;
			}
		}
		
	}

	return sum;
}

constexpr auto part2(auto const& input) {
	return 0;
}
