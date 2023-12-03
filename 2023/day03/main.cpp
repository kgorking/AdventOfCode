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

int part1() {
	// look for symbols
	int line_nr = 0;
	int sum = 0;
	for (std::string_view const line : input) {
		int offset = 0;
		while (true) {
			std::size_t const index = line.find_first_not_of(".0123456789", offset);
			if (index == std::string_view::npos)
				break;

			for (int v : preproccessed[line_nr].at(index))
				sum += v;
			offset = index + 1;
		}

		line_nr += 1;
	}

	return sum;
}

int part2() {
	// look for gears
	int line_nr = 0;
	int sum = 0;
	for (std::string_view const line : input) {
		int offset = 0;
		while (true) {
			std::size_t const index = line.find('*', offset);
			if (index == std::string_view::npos)
				break;

			auto const& vec = preproccessed[line_nr].at(index);
			if (vec.size() == 2)
				sum += (vec[0] * vec[1]);

			offset = index + 1;
		}

		line_nr += 1;
	}

	return sum;
}

int main() {
	std::cout << "Part 1: " << part1() << '\n';
	std::cout << "Part 2: " << part2() << '\n';
}
