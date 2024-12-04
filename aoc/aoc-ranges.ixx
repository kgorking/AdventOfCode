export module aoc:ranges;
import std;

export namespace kg {
	constexpr auto sum = std::bind_back(std::ranges::fold_left, 0, std::plus{});
}

