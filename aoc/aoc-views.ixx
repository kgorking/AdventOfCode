export module aoc:views;
import std;

using namespace std::placeholders;

export namespace kg::views {
export constexpr auto pair_zip_transform = std::bind(std::views::zip_transform, _3, _1, _2);
export constexpr auto indexed_transform = std::bind(std::views::zip_transform, _2, std::views::iota(0), _1);

}
