export module aoc:cursor;
import :direction;
import :pos2d;
import std;

export namespace kg {

class cursor {
public:
	kg::pos2di pos { 0, 0 };
	direction dir { direction::up() };

	// bad idea?
	operator kg::pos2di() const {
		return pos;
	}

	cursor peek_in(direction d) const {
		return { pos + d, d };
	}

	cursor next() const {
		return { pos + dir, dir };
	}

	cursor prev() const {
		return { pos - dir, -dir };
	}

	void step() {
		pos += dir;
	}

	void step_back() {
		pos -= dir;
	}

	kg::pos2di move(direction d) {
		pos += d;
		return pos;
	}

	// Yields all neighbours
	std::generator<cursor> neighbours() const {
		co_yield { pos + dir, dir };
		co_yield { pos + dir.get_turn_cw(), dir.get_turn_cw() };
		co_yield { pos + dir.get_turn_ccw(), dir.get_turn_ccw() };
		co_yield { pos - dir, -dir };
	}

	// Yields neighbours without the one behind the cursor
	std::generator<cursor> neighbours_not_behind() const {
		co_yield { pos + dir, dir };
		direction rdir = dir.get_turn_cw();
		co_yield { pos + rdir, rdir };
		co_yield { pos - rdir, -rdir };
	}

	// Yields neighbours to the left and right
	std::generator<cursor> neighbours_lr() const {
		direction rdir = dir.get_turn_cw();
		co_yield { pos + rdir, rdir };
		co_yield { pos - rdir, -rdir };
	}
};
}
