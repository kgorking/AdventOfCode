export module aoc:shan;
import std;

// short-hand algorithm notation

export namespace kg::short_hand {

auto operator*(std::ranges::input_range auto l, auto&& fn) {
	return std::views::transform(l, fn);
}

auto operator/(std::ranges::view auto l, auto&& fn) {
	auto init = *l.begin();
	return std::reduce(std::next(l.begin()), l.end(), init, fn);
}

} // namespace kg::short_hand
