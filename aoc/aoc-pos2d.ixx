export module aoc : pos2d;
import std;

export namespace kg {

template <typename T = int>
struct pos2d {
	T x = 0, y = 0;

	[[nodiscard]] constexpr pos2d operator+(pos2d const p) const {
		return {x + p.x, y + p.y};
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

	auto operator<=>(pos2d const&) const noexcept = default;
};

} // namespace kg