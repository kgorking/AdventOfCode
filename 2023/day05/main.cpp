import aoc;

struct range_t {
	unsigned dest, src, len;
};

using ranges_t = std::vector<range_t>;
using interval_map_t = std::map<unsigned, unsigned>;
using interval_maps_t = std::array<interval_map_t, 7>;

template <int N>
struct almanac_t {
	std::array<unsigned, N> seeds;
	std::array<ranges_t, 7> maps;
};

const auto almanac = almanac_t<20>{
#include "input.txt"
};

unsigned lookup(interval_map_t const& map, unsigned v) {
	if (v <= map.begin()->first)
		return v;
	auto const it = std::prev(map.upper_bound(v));
	unsigned offset = it->second - it->first;
	return v + offset;
}

unsigned lookup(interval_maps_t const& maps, unsigned const v) {
	unsigned result = v;
	for (size_t i = 0; i < maps.size(); i++) {
		result = lookup(maps[i], result);
	}
	return result;
}

auto build_interval_maps() {
	interval_maps_t interval_maps;

	// Convert the input ranges from the 'almanac' into intervals
	for (std::size_t i = 0; i < almanac.maps.size(); i++) {
		auto& intervals = interval_maps[i];

		for (range_t const& range : almanac.maps[i]) {
			auto const end = range.src + range.len - 1;

			intervals[range.src] = range.dest;
			if (!intervals.contains(end))
				intervals[end] = end;
		}
	}

	// Propagate range interval boundaries upwards. This will result in the top-most
	// map having ~2500 entries, but each additional entry corresponds to
	// a change in a map below it. Instead of iterating millions of seeds,
	// I can just check the positions in the interval map where something changes.
	for (auto i = ptrdiff_t(almanac.maps.size()) - 2; i >= 0; i--) {
		auto& map = interval_maps[i];
		auto const& map_below = interval_maps[i + 1];

		for (auto const& [first, last] : map_below) {
			if (!map.contains(first)) {
				unsigned const split_val = lookup(map, first);
				map[first] = split_val;
			}
			if (!map.contains(last - 1)) {
				unsigned const split_val = lookup(map, last - 1);
				map[last - 1] = split_val;
			}
		}
	}

	return interval_maps;
}

unsigned part1(interval_maps_t const& maps) {
	unsigned min = std::numeric_limits<unsigned>::max();

	for (unsigned val : almanac.seeds)
		min = std::min(min, lookup(maps, val));

	return min;
}

unsigned part2(interval_maps_t const& maps) {
	unsigned min = std::numeric_limits<unsigned>::max();

	for (auto const view : almanac.seeds | std::views::chunk(2)) {
		auto const first_seed = maps[0].upper_bound(view[0]);
		auto const last_seed = maps[0].upper_bound(view[0] + view[1] - 1);

		for (auto it = first_seed; it != last_seed; ++it)
			min = std::min(min, lookup(maps, it->first));
	}

	return min;
}

int main() {
	auto const interval_maps = build_interval_maps();

	std::cout << "Part 1: " << part1(interval_maps) << '\n';
	std::cout << "Part 2: " << part2(interval_maps) << '\n';
}
