export module aoc:grid;
import std;
import :pos2d;
import :views;

export namespace kg {
template <class T>
struct grid {
	using base_type = std::remove_cvref_t<decltype(std::declval<T>()[0][0])>;

	T& ref;
	base_type oob_val = {};

	kg::pos2di find(base_type val) const {
		for (auto [y, line] : ref | std::views::enumerate) {
			for (auto [x, c] : line | std::views::enumerate)
				if (c == val)
					return { (int)x, (int)y };
		}
		return { -1, -1 };
	}

	constexpr auto coords() const {
		return ref | kg::views::coord2d;
	}

	constexpr auto values_and_coords() const {
		return ref | kg::views::with_coord2d;
	}

	constexpr base_type& operator[](kg::pos2di p) requires (!std::is_const_v<T>) {
		if (in_bounds(p)) {
			return ref[p.y][p.x];
		} else {
			return oob_val;
		}
	}

	constexpr base_type operator[](kg::pos2di p) const {
		if (in_bounds(p)) {
			return ref[p.y][p.x];
		} else {
			return oob_val;
		}
	}

	constexpr bool in_bounds(kg::pos2di p) const {
		return (p.x >= 0 && p.y >= 0 && p.y < ref.size() && p.x < ref[0].size());
	}

	constexpr std::size_t width() const {
		return ref[0].size();
	}

	constexpr std::size_t height() const {
		return ref.size();
	}
};
}