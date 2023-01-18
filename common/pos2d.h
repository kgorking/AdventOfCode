export namespace kg {

template <typename T>
struct pos2d {
	T x = 0, y = 0;

	pos2d operator+(pos2d p) const {
		return {x + p.x, y + p.y};
	}

	auto operator<=>(pos2d const&) const noexcept = default;
};

} // namespace kg