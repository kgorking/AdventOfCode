export module aoc : pos2d;
import std;

export namespace kg {

template <typename T = int>
struct pos2d {
	T x = 0, y = 0;

	[[nodiscard]] constexpr pos2d operator+(pos2d const p) const {
		return {x + p.x, y + p.y};
	}
	constexpr void operator+=(pos2d const p) {
		x += p.x;
		y += p.y;
	}
	constexpr void operator-=(pos2d const p) {
		x -= p.x;
		y -= p.y;
	}

	[[nodiscard]] constexpr pos2d operator*(T t) const {
		return {x * t, y * t};
	}

	[[nodiscard]] constexpr pos2d operator-() const {
		return {-x, -y};
	}

	// Move this point towards 'p'
	[[nodiscard]] constexpr bool follow(pos2d const p) {
		int const dx = (p.x - x);
		int const dy = (p.y - y);

		// branchless
		bool const b = (std::abs(dx) | std::abs(dy)) > 1;
		x += b * ((dx > 0) - (dx < 0));
		y += b * ((dy > 0) - (dy < 0));
		return b;
	}

	// Returns number of steps it takes to move to `p`
	[[nodiscard]] constexpr int steps_to(pos2d const p) const {
		auto abs = [](auto v) { return (v < 0) ? -v : +v; };
		return abs(x - p.x) + abs(y - p.y);
	}

	// Rotate the vector ccw
	[[nodiscard]] constexpr pos2d rotated() const {
		return {-y, x};
	}

	// Rotate the vector ccw
	[[nodiscard]] constexpr void rotate() {
		*this = rotated();
	}

	auto operator<=>(pos2d const&) const noexcept = default;
};

using pos2di = pos2d<int>;
using pos2df = pos2d<float>;

} // namespace kg