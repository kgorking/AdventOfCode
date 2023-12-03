import aoc;

using namespace std::string_view_literals;
constexpr auto input = std::to_array<std::string_view>({
#include "input.txt"
});

const auto preproccessed = []() {
	std::array<std::unordered_map<int, std::vector<int>>, input.size()> arr{};

	// Find numbers and spill them into surrounding cells
	int line_nr = 0;
	for (std::string_view const line : input) {
		int offset = 0;
		while (true) {
			std::ptrdiff_t const first = line.find_first_of("0123456789", offset);
			if (first == std::string_view::npos)
				break;
			std::ptrdiff_t const last = std::min(line.size(), line.find_first_not_of("0123456789", first));

			int value = 0;
			std::from_chars(line.data() + first, line.data() + last, value);

			for (ptrdiff_t i = std::max({0ll, first - 1}); i < std::min({std::ssize(line), last + 1}); i++) {
				if (line_nr - 1 >= 0)
					arr[line_nr - 1][i].push_back(value);
				arr[line_nr][i].push_back(value);
				if (line_nr + 1 < arr.size())
					arr[line_nr + 1][i].push_back(value);
			}

			offset = last + 1;
		}

		line_nr += 1;
	}

	return arr;
}();

std::pair<int,int> solve() {
	// look for symbols and gears
	int line_nr = 0;
	int sum_p1 = 0, sum_p2 = 0;

	for (std::string_view const line : input) {
		int offset = 0;
		while (true) {
			std::size_t const index = line.find_first_not_of(".0123456789", offset);
			if (index == std::string_view::npos)
				break;

			std::vector<int> const& vec = preproccessed[line_nr].at(index);
			for (int v : vec)
				sum_p1 += v;

			if ('*' == line[index] && 2 == vec.size())
				sum_p2 += (vec[0] * vec[1]);

			offset = index + 1;
		}

		line_nr += 1;
	}

	return {sum_p1, sum_p2};
}

int main() {
	auto [p1, p2] = solve();
	std::cout << "Part 1: " << p1 << '\n';
	std::cout << "Part 2: " << p2 << '\n';
}
