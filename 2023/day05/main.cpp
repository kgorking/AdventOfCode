import aoc;

struct range_t {
	unsigned dest, src, len;
};

using ranges = std::vector<range_t>;
using interval_map = std::map<unsigned, unsigned>;
using interval_maps_t = std::array<interval_map, 7>;

template <int N>
struct almanac_t {
	std::array<unsigned, N> seeds;
	std::array<ranges, 7> maps;
};

const auto almanac = almanac_t<4>{
#include "input.txt"
};

auto find(interval_map const& map, unsigned v) {
	return std::prev(map.upper_bound(v));
}

unsigned calc_offset(interval_map::const_iterator it, unsigned v) {
	unsigned offset = it->second - it->first;
	return v + offset;
}

unsigned lookup(interval_map const& map, unsigned v) {
	if (v <= map.begin()->first)
		return v;
	return calc_offset(find(map, v), v);
}

auto build_interval_maps() {
	interval_maps_t interval_maps;

	for (std::size_t i = 0; i < almanac.maps.size(); i++) {
		auto& intervals = interval_maps[i];
		for (range_t const& range : almanac.maps[i]) {
			auto const end = range.src + range.len;

			intervals[range.src] = range.dest;
			if (!intervals.contains(end))
				intervals[end] = end;
		}
	}

	// Propagate range barriers upwards
	for (auto i = ptrdiff_t(almanac.maps.size()) - 2; i >= 0; i--) {
		auto& map = interval_maps[i];
		auto const& map_below = interval_maps[i + 1];

		for (auto const& [val, _] : map_below) {
			if (!map.contains(val)) {
				unsigned const split_val = lookup(map, val);
				map[val] = split_val;
			}
		}
	}

	return interval_maps;
}

unsigned part1(interval_maps_t const& maps) {
	unsigned min = std::numeric_limits<unsigned>::max();

	for (unsigned val : almanac.seeds) {
		for (size_t i = 0; i < maps.size(); i++)
			val = lookup(maps[i], val);
		min = std::min(min, val);
	}

	return min;
}

unsigned part2(interval_maps_t const& maps) {
	unsigned min = std::numeric_limits<unsigned>::max();

	for (auto const view : almanac.seeds | std::views::chunk(2)) {
		unsigned const first = view[0]; // seed
		unsigned const last = view[0] + view[1] - 1;

		auto const begin = std::prev(maps[0].upper_bound(first));
		auto const end = maps[0].upper_bound(last);

		for (auto it = begin; it != end; ++it) {
			unsigned val = std::max(first, it->first);
			for (size_t i = 0; i < maps.size(); i++) {
				val = lookup(maps[i], val);
			}
			min = std::min(min, val);
		}
	}

	//   662197086
	// < 199168760
	return min;
}

int main() {
	auto const interval_maps = build_interval_maps();

	std::cout << "Part 1: " << part1(interval_maps) << '\n';
	std::cout << "Part 2: " << part2(interval_maps) << '\n';
}
