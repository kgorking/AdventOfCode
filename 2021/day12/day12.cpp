#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Constants for start and end vertices
constexpr int8_t v_start = std::numeric_limits<int8_t>::min();
constexpr int8_t v_end = 1 + v_start;

struct edge {
	int8_t v1, v2;
};

using adjacency_list = std::unordered_map<int8_t, std::unordered_set<int8_t>>;
using visited_small_caves = std::bitset<10>;

// Builds an adjacency list from the edges
adjacency_list make_adjacency_list() {
	constexpr std::array<edge, 25> input{{
#include "input.txt"
	}};

	adjacency_list al;

	for (edge const e : input) {
		al[e.v1].insert(e.v2);
		al[e.v2].insert(e.v1);
	}

	return al;
}

// Paths to the neighbours of the 'current' vertex and returns the number of times
// the end vertex was reached.
// Visits to small caves are stored in 'vis' and are skipped
// unless 'extra_visit' is true.
int count_paths_to_end(adjacency_list const& adj, int8_t current, visited_small_caves vis, bool const extra_visit = false) {
	int ends_found = 0;

	// Visit neighbours
	for (int8_t const next : adj.at(current)) {
		// Can't go back to the start
		if (next == v_start)
			continue;

		// End is reached
		if (next == v_end) {
			ends_found += 1;
			continue;
		}

		// Small cave test. Vertices between 0 and 9 are small caves.
		bool const small_cave = (next >= 0 && next <= 9);
		if (small_cave) {
			// Skip if already visited
			if (vis.test(next)) {
				// If an extra visit is allowed, continue to the vertex
				// and disallow further extra visits. This ensure that
				// only a single small cave is visited twice on the path.
				if (extra_visit) {
					ends_found += count_paths_to_end(adj, next, vis, false);
				}

				continue;
			} else {
				// Mark the cave as visited
				vis.set(next);
			}
		}

		// Continue the path to the neighbouring vertex
		ends_found += count_paths_to_end(adj, next, vis, extra_visit);

		// Unmark the small cave as visited so other neighbours
		// can also visit it.
		if (small_cave)
			vis.reset(next);
	}

	return ends_found;
}

int main() {
	// Build the adjacency list
	adjacency_list const adj = make_adjacency_list();

	// Part 1
	int const paths1 = count_paths_to_end(adj, v_start, {});
	std::cout << "Part 1: " << paths1 << '\n';

	// Part 2
	int const paths2 = count_paths_to_end(adj, v_start, {}, true);
	std::cout << "Part 2: " << paths2 << '\n';
}
