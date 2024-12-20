export module aoc.day14;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(12, 0);
export constexpr auto expected_input = std::make_pair(232253028, 8179);

constexpr int quadrant(kg::pos2di p) {
	return 2 * std::signbit(p.y) + std::signbit(p.x);
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
		counts[quadrant(p - hdim)] += (p.x != hdim.x && p.y != hdim.y);
	}

	return counts[0] * counts[1] * counts[2] * counts[3];
}

export constexpr auto part2(auto&& input) {
	if (input.size() == 12) // doesn't work with sample data
		return 0;

	int const num_robots = input.size();
	kg::pos2di constexpr dim { 101, 103 };
	kg::pos2di constexpr hdim { dim.x / 2, dim.y / 2 };

	auto robots = input;
	std::array<int, 4> counts;

	int last_fac = 0;
	for (int const second : std::views::iota(1)) {
		counts.fill(0);

		for (auto& [p, v] : robots) {
			p += v;
			p %= dim;
			counts[quadrant(p - hdim)] += (p.x != hdim.x && p.y != hdim.y);
		}

		int const safety_factor = counts[0] * counts[1] * counts[2] * counts[3];

		// Check for safety factor fluctuations
		if (safety_factor < std::exchange(last_fac, safety_factor) / 4) {
			std::ranges::sort(robots, [](auto const& l, auto const& r) { return l[0].y < r[0].y; });
			for (int y = 0; y < dim.y; y++) {
				// Count the number of robots with the same y-value
				int const orig_y = y;
				while (robots[y][0].y == robots[orig_y][0].y)
					y += 1;

				if ((y - orig_y) >= 25)
					return second;
			}
		}
	}

	return 0;
}
