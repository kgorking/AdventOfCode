export module aoc:grid;
import std;
import :pos2d;
import :views;

export namespace kg {
template <class T>
struct grid {
	using base_type = std::remove_cvref_t<decltype(std::declval<T>()[0][0])>;

	T& ref;
	base_type oob_val = '\0';

	constexpr auto coords() const {
		return ref | kg::views::coord2d;
	}

	constexpr auto values_and_coords() const {
		return ref | kg::views::with_coord2d;
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