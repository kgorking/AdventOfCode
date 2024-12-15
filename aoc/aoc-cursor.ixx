export module aoc:cursor;
import :pos2d;
import std;

export namespace kg {
enum class direction { up, right, down, left };

class cursor {
public:
	static constexpr auto directions = std::to_array<kg::pos2di>({ { 0, -1 }, { 1, 0 }, { 0, +1 }, { -1, 0 } });

	kg::pos2di pos { 0, 0 };
	direction dir { direction::up };

	operator kg::pos2di() const {
		return pos;
	}

	cursor peek_in(direction d) const {
		return { pos + directions[(int)d], d };
	}

	void step() {
		pos += directions[(int)dir];
	}

	void step_back() {
		pos -= directions[(int)dir];
	}

	kg::pos2di move(direction d) {
		pos += directions[(int)d];
		return pos;
	}
};
}
