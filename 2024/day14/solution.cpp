export module aoc.day14;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(12, 0);
export constexpr auto expected_input = std::make_pair(232253028, 8179);

constexpr int quadrant(kg::pos2di p) {
	return 3 - (2 * std::signbit(p.y) + std::signbit(p.x));
}

export auto part1(auto&& input) {
	int const w = input.size() == 12 ? 11 : 101;
	int const h = input.size() == 12 ? 7 : 103;
	kg::pos2di const dim { w, h };
	kg::pos2di const hdim { w / 2, h / 2 };

	std::array<int, 4> counts;
	counts.fill(0);

	for (auto [p, v] : input) {
		p += v * 100;
		p %= dim;

		if (p.x != hdim.x && p.y != hdim.y)
			counts[quadrant(p - hdim)] += 1;
	}

	return counts[0] * counts[1] * counts[2] * counts[3];
}

export constexpr auto part2(auto&& input) {
	if (input.size() == 12) // doesn't work with sample data
		return 0;

	kg::pos2di constexpr dim { 101, 103 };
	kg::pos2di constexpr hdim { dim.x / 2, dim.y / 2 };

	auto robots = input;
	std::array<int, 4> counts;

	for (int const second : std::views::iota(1)) {
		counts.fill(0);

		for (auto& [p, v] : robots) {
			p += v;
			p %= dim;

			if (p.x != hdim.x && p.y != hdim.y)
				counts[quadrant(p - hdim)] += 1;
		}

		if (counts[1] + counts[3] > 2 * (counts[0] + counts[2])) {
			std::ranges::sort(robots, [](auto const& l, auto const& r) { return l[0].y < r[0].y; });
			for (int y = 0; y < dim.y; y++) {
				int orig_y = y;
				while (robots[y][0].y == robots[orig_y][0].y) {
					y += 1;
				}

				if ((y - orig_y) >= 25)
					return second;
			}
		}
	}

	return 0;
}
