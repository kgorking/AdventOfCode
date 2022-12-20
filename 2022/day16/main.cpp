import std;
#include "../../common/bits.h"

struct valve {
	std::string_view name;
	int flow_rate;
	std::vector<std::string_view> connections;
};

const auto input = std::to_array<valve>({
#if 1
#include "input.txt"
#else
	{{"AA"}, 0, {"DD", "II", "BB"}}, {{"BB"}, 13, {"CC", "AA"}}, {{"CC"}, 2, {"DD", "BB"}}, {{"DD"}, 20, {"CC", "AA", "EE"}},
		{{"EE"}, 3, {"FF", "DD"}}, {{"FF"}, 0, {"EE", "GG"}}, {{"GG"}, 0, {"FF", "HH"}}, {{"HH"}, 22, {"GG"}}, {{"II"}, 0, {"AA", "JJ"}}, {
		{"JJ"}, 21, {
			"II"
		}
	}
#endif
});

template <typename T, int N>
using matrix = std::array<std::array<T, N>, N>;
template <typename T, int N>
using adjacency_list_t = std::array<std::vector<T>, N>;
using adjacency_list = adjacency_list_t<short, input.size()>;
using valve_set = std::bitset<input.size()>;

adjacency_list build_adjacency_list() {
	// Convert cave names to indices
	std::unordered_map<std::string_view, short> name_index;
	for (valve const& v : input)
		name_index[v.name] = static_cast<short>(std::distance(input.data(), &v));

	adjacency_list adj;
	for (valve const& v : input) {
		short const index = name_index[v.name];
		for (auto sv : v.connections)
			adj[index].push_back(name_index[sv]);
	}

	return adj;
}

// Returns a matrix with the shortest paths between all valves,
// in a snappy O(N^3) instant
template <typename T, int N>
matrix<T, N> build_shortest_path_matrix(adjacency_list_t<T, N> const& adj) {
	matrix<T, N> minutes;
	// std::memset(&minutes, 99, sizeof(minutes));

	// Initialize the matrix from the adjacency graph
#if 0
	for (std::size_t i = 0; i < adj.size(); i++) {
		minutes[i].fill(89);
		for (int j : adj[i])
			minutes[i][j] = 1;
	}
	for (std::size_t d = 0; d < N; d++)
		minutes[d][d] = 0;
#else
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			minutes[i][j] = (i != j) * std::numeric_limits<T>::max() / 2;
		}
		for (int j : adj[i])
			minutes[i][j] = 1;
	}
#endif
	// Fill in remaining minutes between valves
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				minutes[i][j] = std::min<T>(minutes[i][j], minutes[i][k] + minutes[k][j]);
			}
		}
	}

	return minutes;
}

struct traversal_state {
	// remaining time
	int time = 0;

	// current pressure
	int pressure = 0;

	// current valve
	int valve = 0;

	// valves that have been opened
	valve_set open_valves = 0;
};

template <typename T, int N>
traversal_state find_best_pressure(matrix<T, N> const& shortest_paths, valve_set const valves, int start_valve, int time) {
	// Load up the initial state in the queue
	std::stack<traversal_state> queue;
	queue.emplace(time - (input[start_valve].flow_rate > 0), 0, start_valve, valve_set{1} << start_valve);

	// Traverse the valves
	traversal_state max_state;
	while (!queue.empty()) {
		traversal_state state = queue.top();
		queue.pop();

		if (state.pressure > max_state.pressure)
			max_state = state;

		auto const closed_valves = valves ^ state.open_valves;
		int valve = kg::find_first_set(closed_valves);
		while (valve < closed_valves.size()) {
			auto neighbour = state;
			neighbour.time -= 1 + shortest_paths[state.valve][valve];
			if (neighbour.time >= 0) {
				neighbour.pressure += input[valve].flow_rate * neighbour.time;
				neighbour.valve = valve;
				neighbour.open_valves.set(valve);
				queue.push(neighbour);
			}

			valve = kg::bit_position_left(closed_valves, valve);
		}
	}

	return max_state;
}

int main() {
	// Create an adjacency list for the valves
	auto const adj_list = build_adjacency_list();

	// Create the shortest-path minute-cost matrix
	auto const shortest_paths = build_shortest_path_matrix(adj_list);

	// The 'interresting' valves
	valve_set valves;
	for (std::size_t i = 0; i < input.size(); i++) {
		if (input[i].flow_rate > 0)
			valves.set(i);
	}

	auto const max_pressure_30 = find_best_pressure(shortest_paths, valves, 0, 30);

	// Part 1
	// > 1836
	// ! 2165
	// ! 2176
	// ! 2285
	// < 2296
	// < 2484
	std::cout << "Part 1: " << max_pressure_30.pressure << '\n';

	// Part 2
	std::cout << "Part 2: " << 0 << '\n';
}
