#pragma once
#include <array>

enum class city {
	AlphaCentauri,
	Snowdin,
	Tambi,
	Faerun,
	Norrath,
	Straylight,
	Tristram,
	Arbre
};

using distance = std::tuple<city, city, int>;

std::array<distance, 28> constexpr distances{
	distance{city::AlphaCentauri, city::Snowdin, 66},
	{city::AlphaCentauri, city::Tambi, 28},
	{city::AlphaCentauri, city::Faerun, 60},
	{city::AlphaCentauri, city::Norrath, 34},
	{city::AlphaCentauri, city::Straylight, 34},
	{city::AlphaCentauri, city::Tristram, 3},
	{city::AlphaCentauri, city::Arbre, 108},
	{city::Snowdin, city::Tambi, 22},
	{city::Snowdin, city::Faerun, 12},
	{city::Snowdin, city::Norrath, 91},
	{city::Snowdin, city::Straylight, 121},
	{city::Snowdin, city::Tristram, 111},
	{city::Snowdin, city::Arbre, 71},
	{city::Tambi, city::Faerun, 39},
	{city::Tambi, city::Norrath, 113},
	{city::Tambi, city::Straylight, 130},
	{city::Tambi, city::Tristram, 35},
	{city::Tambi, city::Arbre, 40},
	{city::Faerun, city::Norrath, 63},
	{city::Faerun, city::Straylight, 21},
	{city::Faerun, city::Tristram, 57},
	{city::Faerun, city::Arbre, 83},
	{city::Norrath, city::Straylight, 9},
	{city::Norrath, city::Tristram, 50},
	{city::Norrath, city::Arbre, 60},
	{city::Straylight, city::Tristram, 27},
	{city::Straylight, city::Arbre, 81},
	{city::Tristram, city::Arbre, 90},
};
