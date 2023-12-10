import aoc;

//using history = std::array<std::int64_t, 6>;
using history = std::array<std::int64_t, 21>;

constexpr auto dataset = std::to_array<history>({
#include "input.txt"
});

auto solve() {
	auto pair_diff = std::views::slide(2) | std::views::transform([](auto pair) { return pair[1] - pair[0]; });
	//auto pair_diff = std::views::pairwise_transform([](auto a, auto b) { return b - a; });

	std::int64_t total_history[2] = {0, 0};

	for (history const& h : dataset) {
		auto values = std::vector<std::int64_t>(h.begin(), h.end());
		auto front_history = std::vector<std::int64_t>{{h.front()}};
		auto back_history = h.back();

		while (true) {
			auto difference = values | pair_diff;
			if (std::ranges::all_of(difference, std::bind_front(std::equal_to<>{}, 0)))
				break;

			front_history.push_back(difference.front());
			back_history += difference.back();

			std::ranges::copy(difference, values.begin());
			values.pop_back();
		}

		auto val = 0;
		for (auto v : front_history | std::views::reverse) {
			val = v-val;
		}

		total_history[0] += back_history;
		total_history[1] += val;
	}

	return std::make_pair(total_history[0], total_history[1]);
}


int main() {
	auto const [p1, p2] = solve();
	std::cout << "Part 1: " << p1 << '\n';
	std::cout << "Part 2: " << p2 << '\n';
}
