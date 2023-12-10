import aoc;

// using history = std::array<std::int64_t, 6>;
using history = std::array<std::int64_t, 21>;

constexpr auto dataset = std::to_array<history>({
#include "input.txt"
});

auto solve() {
	std::int64_t total_history[2] = {0, 0};

	for (history const& h : dataset) {
		auto values = h;
		auto vbegin = values.begin();
		auto front_history = history{h.front()};
		auto fh_it = ++front_history.begin();

		while (true) {
			std::adjacent_difference(vbegin, values.end(), vbegin);
			if (std::all_of(++vbegin, values.end(), std::bind_front(std::equal_to<>{}, 0)))
				break;

			*fh_it++ = *vbegin;
			total_history[0] += values.back();
		}

		auto val = 0;
		do { 
			val = *--fh_it - val;
		} while (fh_it != front_history.begin());

		total_history[0] += h.back();
		total_history[1] += val;
	}

	return std::make_pair(total_history[0], total_history[1]);
}

int main() {
	auto const [p1, p2] = solve();
	std::cout << "Part 1: " << p1 << '\n';
	std::cout << "Part 2: " << p2 << '\n';
}
