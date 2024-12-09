export module aoc.day06;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(41, 6);
export constexpr auto expected_input = std::make_pair(4964, 1740);

using visitmap = std::map<kg::pos2di, std::set<kg::pos2di>>;
using input_t = std::span<std::string_view const>;

kg::pos2di find_guard_pos(input_t input) {
	for (int y = 0; y < input.size(); y++) {
		for (int x = 0; x < input[0].size(); x++) {
			char const c = input[y][x];
			if (c == '^')
				return { x, y };
		}
	}

	std::unreachable();
}

export auto find_visited(input_t input, bool count_loops) {
	auto is_valid = [&input](kg::pos2di p) {
		return p.x >= 0 && p.x < input[0].size() && p.y >= 0 && p.y < input.size();
	};
	auto at = [&input](kg::pos2di p) {
		return input[p.y][p.x];
	};
	auto step = [&](kg::pos2di& p, kg::pos2di& dir, std::optional<kg::pos2di> obstruction = {}) {
		kg::pos2di next = p + dir;

		// oob
		if (!is_valid(next))
			return false;

		// turn if facing an obstruction
		if (at(next) == '#' || (obstruction.has_value() && next == obstruction)) {
			dir.rotate();
		} else {
			p = next;
		}

		return true;
	};

	// Patrol the area
	kg::pos2di const guard_start_pos = find_guard_pos(input);

	// Part 1
	int loops = 0;
	visitmap visited;
	{
		kg::pos2di dir { 0, -1 };
		kg::pos2di p = guard_start_pos;
		do {
			visited[p].insert(dir);
#if 0
			if (!count_loops)
				continue;

			kg::pos2di const obstruction = p + dir;
			if (!is_valid(obstruction) || at(obstruction) == '#')
				continue;

			kg::pos2di loop_p = p;
			kg::pos2di loop_dir = dir;
			visitmap loop_visited;

			while (step(loop_p, loop_dir, obstruction)) {
				if (visited.contains(loop_p) && visited[loop_p].contains(loop_dir)) {
					loops += 1;
					break;
				}
				if (loop_visited.contains(loop_p) && loop_visited[loop_p].contains(loop_dir)) {
					loops += 1;
					break;
				}
				loop_visited[loop_p].insert(loop_dir);
			}
#endif
		} while (step(p, dir));
	}

	// Part 2
	if (count_loops && loops == 0) {
		for (auto const [obstruction, dirs] : visited) {
			if (obstruction == guard_start_pos)
				continue;
			kg::pos2di p = guard_start_pos;
			kg::pos2di dir = { 0, -1 };

			visitmap loop_visited;
			while (step(p, dir, obstruction)) {
				if (loop_visited.contains(p) && loop_visited[p].contains(dir)) {
					loops += 1;
					break;
				}
				loop_visited[p].insert(dir);
			}
		}
	}

	return std::pair { visited.size(), loops };
}

export auto part1(input_t input) {
	return find_visited(input, false).first;
}

export auto part2(input_t input) {
	return find_visited(input, true).second;
}
