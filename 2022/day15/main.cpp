import std;

#include "../../common/pos2d.h"
#include "../../common/range.h"

using pos = kg::pos2d<int>;
using segment = kg::range<pos>;

struct pair {
	pos sensor, beacon;
};

constexpr auto input = std::to_array<pair>({
#include "input.txt"
});

constexpr int ce_abs(int i) {
	return (i < 0) ? -i : i;
}

constexpr int distance(pair const& p) {
	return ce_abs(p.sensor.x - p.beacon.x) + ce_abs(p.sensor.y - p.beacon.y);
}

constexpr pos get_line_intersection(pos p0, pos p1, pos p2, pos p3) {
	kg::pos2d<double> const s1(p1.x - p0.x, p1.y - p0.y);
	kg::pos2d<double> const s2(p3.x - p2.x, p3.y - p2.y);

	double const det = -s2.x * s1.y + s1.x * s2.y;
	double const s = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / det;
	double const t = (+s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x)) / det;

	// if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	//  never fails, thanks to the bound checks in 'find_sensor_segments'
	return pos{p0.x + static_cast<int>(t * s1.x), p0.y + static_cast<int>(t * s1.y)};
}

struct sensor_area {
	sensor_area(pos p, int dist) : t{p + pos{0, -dist}}, l{p + pos{-dist, 0}}, r{p + pos{dist, 0}}, b{p + pos{0, +dist}} {}

	pos t, l, r, b;
};

template <typename T>
void combine_overlapping_segments(std::vector<kg::range<T>>& segments) {
	std::size_t j = 0;
	for (std::size_t i = 1; i < segments.size(); i++) {
		if (segments[j].overlaps(segments[i])) {
			segments[j] = kg::range<T>::overlapping(segments[i], segments[j]);
		} else {
			j++;
			segments[j] = segments[i];
		}
	}
	segments.resize(j + 1);
}

std::vector<segment> find_sensor_segments(std::vector<sensor_area> const& sensor_areas, pos p_start, pos p_end) {
	std::vector<segment> segments;

	for (sensor_area const& area : sensor_areas) {
		// Test against top of area
		if (p_start.y > area.t.y && p_start.y < area.l.y) {
			pos const int_tl = get_line_intersection(p_start, p_end, area.l, area.t);
			pos const int_tr = get_line_intersection(p_start, p_end, area.r, area.t);
			segment const s{int_tl, int_tr};
			segments.push_back(s);
			continue;
		}

		// Test against bottom of area
		if (p_start.y > area.l.y && p_start.y < area.b.y) {
			pos const int_bl = get_line_intersection(p_start, p_end, area.l, area.b);
			pos const int_br = get_line_intersection(p_start, p_end, area.r, area.b);
			segment const s{int_bl, int_br};
			segments.push_back(s);
		}
	}

	combine_overlapping_segments(segments);

	return segments;
}

int main() {
	int min_x = 0;
	int max_x = 0;
	int const test_row = 2'000'000;

	// Convert sensors
	std::vector<sensor_area> sensor_areas;
	for (pair const& p : input) {
		int const dist = distance(p);
		min_x = std::min(min_x, p.sensor.x - dist);
		max_x = std::max(max_x, p.sensor.x + dist);
		sensor_areas.emplace_back(p.sensor, dist);
	}
	std::ranges::sort(sensor_areas, [](sensor_area const& l, sensor_area const& r) {
		return l.t.x < r.t.x;
	});

	// Part 1
	// Find segments in the sensor areas
	int count = 0;
	pos const row_start{min_x - 1, test_row};
	pos const row_end{max_x + 1, test_row};

	auto const segments = find_sensor_segments(sensor_areas, row_start, row_end);

	// Find positions of beacons
	std::set<pos> remove;
	for (pair const& s_b : input) {
		if (s_b.beacon.y != test_row)
			continue;

		remove.insert(s_b.beacon);
	}

	// Count the available spots, minus the beacons
	for (segment const& s : segments) {
		count += 1 + (s.last.x - s.first.x);

		for (pos p : remove) {
			if (s.contains(p)) {
				count -= 1;
				break;
			}
		}
	}

	std::cout << "Part 1: " << count << '\n';

	// Part 2
	std::cout << "Part 2: ";
	for (int y = 0; y < 4'000'000; y++) {
		pos const p2_start{min_x, y};
		pos const p2_end{max_x, y};

		auto const segments = find_sensor_segments(sensor_areas, p2_start, p2_end);

		int x1 = 0, x2 = 4'000'000;
		for (segment const& s : segments) {
			if (s.overlaps(pos{x1, y})) {
				x1 = s.last.x + 1;
			}
			if (s.overlaps(pos{x2, y})) {
				x2 = s.first.x - 1;
			}

			if (x2 <= x1)
				break;
		}

		if (x1 == x2) {
			std::cout << y + (std::int64_t{x1} * 4'000'000) << '\n';
			break;
		}
	}
}
