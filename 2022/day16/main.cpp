import std;

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

	// Initialize the matrix from the adjacency graph
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j)
				minutes[i][j] = 0;
			else if (std::find(adj[i].begin(), adj[i].end(), j) != adj[i].end())
				minutes[i][j] = 1;
			else
				minutes[i][j] = std::numeric_limits<T>::max() / 2;
		}
	}

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
	int time;

	// current pressure
	int pressure;

	// current valve
	int valve;

	// valves that are still closed
	valve_set closed_valves;

	// for sorting
	bool operator<(traversal_state const& ts) {
		return time > ts.time;
	}
};

template <typename T, int N>
int find_best_pressure(adjacency_list const& adj, matrix<T, N> const& shortest_paths, std::vector<std::size_t> const& valves,
					   int start_valve, int time) {
	traversal_state initial{time, 0, start_valve};
	for (auto i : valves)
		;
	std::priority_queue<traversal_state> queue;
	return 0;
}

int main() {
	// Create an adjacency list for the valves
	auto const adj_list = build_adjacency_list();

	// Create the shortest-path minute-cost matrix
	auto const shortest_paths = build_shortest_path_matrix(adj_list);

	// The 'interresting ' valves
	std::vector<std::size_t> valves;
	for (std::size_t i = 0; i < input.size(); i++) {
		if (input[i].flow_rate > 0)
			valves.push_back(i);
	}
	std::ranges::sort(valves);

	std::size_t max_pressure = 0;
	do {
		std::size_t last_valve = 0;
		std::size_t total_pressure = 0;
		std::size_t total_time = 0;
		for (std::size_t v : valves) {
			total_time += 1 + shortest_paths[last_valve][v];
			last_valve = v;
			if (total_time > 30) {
				break;
			}
			total_pressure += input[v].flow_rate * (30 - total_time);
		}

		max_pressure = std::max(max_pressure, total_pressure);
	} while (std::next_permutation(valves.begin(), valves.end()));

	// Part 1
	std::cout << "Part 1: " << max_pressure << '\n';

	// Part 2
	std::cout << "Part 2: " << 0 << '\n';
}
