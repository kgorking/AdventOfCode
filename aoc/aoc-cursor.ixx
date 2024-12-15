export module aoc:cursor;
import :direction;
import :pos2d;
import std;

export namespace kg {

class cursor {
public:
	kg::pos2di pos { 0, 0 };
	direction dir { direction::up };

	operator kg::pos2di() const {
		return pos;
	}

	cursor peek_in(direction d) const {
		return { pos + d, d };
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
};
}
