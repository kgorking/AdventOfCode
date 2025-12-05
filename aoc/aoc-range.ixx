export module aoc : range;
import std;

export namespace kg {

template <typename T>
struct range {
	T first, last;

	[[nodiscard]] constexpr T size() const {
		return (last - first) + 1;
	}
	[[nodiscard]] constexpr bool contains(T const& t) const {
		return t >= first && t <= last;
	}
	[[nodiscard]] constexpr bool contains(range const& other) const {
		return other.first >= first && other.last <= last;
	}
	[[nodiscard]] constexpr bool overlaps(T const& t) const {
		return first <= t && t <= last;
	}
	[[nodiscard]] constexpr bool overlaps(range const& other) const {
		return first <= other.last && other.first <= last;
	}
	// Returns the direction of the range
	[[nodiscard]] constexpr T direction() const {
		T dir = last - first;
		dir.x /= size();
		dir.y /= size();
		return dir;
	}

	[[nodiscard]] constexpr static range merge(range const& r1, range const& r2) {
		//Expects(r1.adjacent(r2));
		return range { std::min(r1.first, r2.first), std::max(r1.last, r2.last) };
	}
	// Returns a range that overlaps the two ranges
	[[nodiscard]] constexpr static range overlapping(range const& r1, range const& r2) {
		T const first{std::min(r1.first, r2.first)};
		T const last{std::max(r1.last, r2.last)};

		return range{first, last};
	}

	// Splits a range into [first, split_pos-1], [split_pos, last]
	[[nodiscard]] constexpr static std::pair<range,range> split(range const& r, T split_pos) {
		if (!r.contains(split_pos) || r.first == split_pos || r.last == split_pos)
			throw std::invalid_argument("splitting position must be in range");
		return {{r.first, split_pos - 1}, {split_pos, r.last}};
	}

	auto operator<=>(range const&) const noexcept = default;
};

template <typename T>
struct range_pair {
	range<T> r1, r2;

	[[nodiscard]] constexpr bool one_contains_other() const {
		return r1.contains(r2) || r2.contains(r1);
	}
	[[nodiscard]] constexpr bool one_overlaps_other() const {
		return r1.overlaps(r2) || r2.overlaps(r1);
	}
};

} // namespace kg