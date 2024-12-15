export module aoc.day15;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(10092, 9021);
export constexpr auto expected_input = std::make_pair(1577255, 1597035);

export template <int N>
struct input_t {
	std::array<std::string, N> map;
	std::string_view moves;
};

using map_view = std::span<std::string>;

// Find robot in map
kg::pos2di find_robot(map_view const map) {
	kg::pos2di p { -1, -1 };
	for (auto const& [y, str] : map | std::views::enumerate) {
		p.x = str.find('@');
		if (p.x != -1) {
			p.y = y;
			break;
		}
	}

	return p;
}

kg::direction char_to_dir(char c) {
	switch (c) {
	case '^': return kg::direction::up;
	case '>': return kg::direction::right;
	case 'v': return kg::direction::down;
	case '<': return kg::direction::left;
	}
	std::unreachable();
}

export template <int N>
std::int64_t part1(input_t<N> input) {
	auto dirs = input.moves | std::views::transform(char_to_dir);
	auto map = kg::grid { input.map };
	auto robot = kg::cursor { find_robot(input.map), kg::direction::up };

	for (kg::direction const dir : dirs) {
		kg::cursor const next = robot.peek_in(dir);

		// Empty space, so move there
		if (map[next] == '.') {
			std::swap(map[robot], map[next]); // move the '@' in map
			robot = next;
		} else if (map[next] == '#') {
			// Hit a wall, so do nothing
		} else {
			// Search for an empty spot behind crate
			kg::cursor search = next;
			while (map[search] == 'O')
				search.step();

			if (map[search] == '.') {
				// Found an empty space, so move the box
				std::swap(map[search], map[next]);

				// Move the robot
				std::swap(map[robot], map[next]); // move the '@' in map
				robot = next;
			} else {
				// Hit a wall, so do nothing
			}
		}
	}

	return kg::sum(map.values_and_coords() | kg::views::filter_equal('O', kg::select<0>) | std::views::values
				   | std::views::transform([](kg::pos2di p) { return p.y * 100 + p.x; }));
}

export template <int N>
std::int64_t part2(input_t<N> input) {
	// Widen the map
	std::array<std::string, input.map.size()> wide_map;
	for (auto [i, s] : input.map | std::views::enumerate) {
		for (char c : s) {
			switch (c) {
			case '#': wide_map[i] += "##"; break;
			case 'O': wide_map[i] += "[]"; break;
			case '.': wide_map[i] += ".."; break;
			case '@': wide_map[i] += "@."; break;
			default: throw;
			}
		}
	}

	auto map = kg::grid { wide_map };
	auto robot = kg::cursor { find_robot(wide_map), kg::direction::up };

	for (auto const [i, ch_mov] : input.moves | std::views::enumerate) {
		kg::direction const dir = char_to_dir(ch_mov);
		kg::cursor const next = robot.peek_in(dir);

		// Empty space, so move there
		if (map[next] == '.') {
			std::swap(map[robot], map[next]); // move the '@' in map
			robot = next;
		} else if (map[next] == '#') {
			// Hit a wall, so do nothing
		} else {
			// Search for an empty spot behind crate
			// Found an empty space, so move the box
			if (dir == kg::direction::left || dir == kg::direction::right) {
				kg::cursor search = next;
				while (map[search] == ']' || map[search] == '[')
					search.step();

				if (map[search] == '.') {
					while (search.pos != next.pos) {
						auto const prev = search.pos;
						search.step_back();
						std::swap(map[search], map[prev]);
					}

					// Move the robot
					std::swap(map[robot], map[next]); // move the '@' in map
					robot = next;
				}
			} else {
				kg::pos2di const v_dir = kg::cursor::directions[(int)dir];

				// move up/down
				//  [][][]
				//   [][]
				//    []
				//    @
				// one move -> 6 boxes moves
				auto can_move = [&](this auto& self, kg::pos2di p) {
					auto p2 = p;
					p2.x += (map[p] == ']') ? -1 : +1;

					p += v_dir;
					p2 += v_dir;

					char const c1 = map[p];
					char const c2 = map[p2];

					if (c1 == '.' && c2 == '.')
						return true;
					if (c1 == '#' || c2 == '#')
						return false;

					if (c1 == '.')
						return self(p2);
					else if (c2 == '.')
						return self(p);
					else
						return self(p) && self(p2);
				};

				auto do_move = [&](this auto& self, kg::pos2di p) -> void {
					auto p2 = p;
					p2.x += (map[p] == ']') ? -1 : +1;

					char const& c1 = map[p + v_dir];
					char const& c2 = map[p2 + v_dir];

					if (c1 == ']' || c1 == '[')
						self(p + v_dir);
					if (c2 == ']' || c2 == '[')
						self(p2 + v_dir);

					if (c1 == '.' && c2 == '.') {
						std::swap(map[p], map[p + v_dir]);
						std::swap(map[p2], map[p2 + v_dir]);
					}
				};

				if (can_move(next)) {
					do_move(next);
					std::swap(map[robot], map[next]); // move the '@' in map
					robot = next;
				}
			}
		}
	}

	return kg::sum(map.values_and_coords()
		| kg::views::filter_equal('[', kg::select<0>)
		| std::views::values
		| std::views::transform([](kg::pos2di p) { return p.y * 100 + p.x; }));
}
