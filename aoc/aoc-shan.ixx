export module aoc:shan;
import std;

export namespace kg::short_hand {

auto operator*(std::ranges::input_range auto const& l, auto&& fn) {
	return std::views::transform(l, fn);
}

auto operator+(std::ranges::input_range auto const& l, auto init) {
	return std::accumulate(l.begin(), l.begin(), init);
}

// operator *
// operator +
} // namespace kg::short_hand
