export module aoc:direction;
import std;
import :pos2d;

export namespace kg {

class direction {
	std::uint8_t d;

	static constexpr auto lookup = std::to_array<kg::pos2di>({ { 0, -1 }, { 1, 0 }, { 0, +1 }, { -1, 0 } });

public:
	static constexpr std::uint8_t up = 0;
	static constexpr std::uint8_t right = 1;
	static constexpr std::uint8_t down = 2;
	static constexpr std::uint8_t left = 3;

	constexpr direction(int dir) : d(dir) {
		if (dir > left)
			throw std::invalid_argument("Invalid direction");
	}

	constexpr bool is_horizontal() const {
		return d == left || d == right;
	}

	constexpr bool is_vertical() const {
		return !is_horizontal();
	}

	// Turn clockwise
	constexpr void turn_cw() {
		d = (d + 1) % 4;
	}

	// Turn counter-clockwise
	constexpr void turn_ccw() {
		d = (d + 3) % 4;
	}

	constexpr direction operator-() const {
		return { (d + 2) % 4 };
	}

	constexpr operator std::uint8_t() const {
		return d;
	}

	template<typename T>
	constexpr operator kg::pos2d<T>() const {
		return lookup[d];
	}
};
}
