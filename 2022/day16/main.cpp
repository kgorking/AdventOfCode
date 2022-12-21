import std;
#include "../../common/bits.h"
#include "../../common/matrix.h"

struct valve_input {
	std::string_view name;
	int flow_rate;
	std::vector<std::string_view> connections;
};

const auto input = std::to_array<valve_input>({
#if 1
#include "input.txt"
#else
#include "example.txt"
//#include "testcase1.txt"
//#include "testcase2.txt"
//#include "testcase3.txt"
//#include "testcase4.txt"
#endif
});


constexpr std::size_t N = input.size();
using T = short;
using matrix = kg::matrix_t<T, N>;
using bitset = std::bitset<N>;


// Create an adjacency matrix for the valves
matrix build_adjacency_matrix(T inf = 100) {
	// Convert valve names to indices
	std::unordered_map<std::string_view, T> name_index;
	for (valve_input const& v : input)
		name_index[v.name] = static_cast<T>(std::distance(input.data(), &v));

	// Fill the matrix with "inf" values
	matrix adj;
	for (auto& row : adj)
		row.fill(inf);

	// Set the edge distances (minutes)
	for (valve_input const& v : input) {
		T const u = name_index[v.name];
		for (auto sv : v.connections) {
			T const v = name_index[sv];
			adj[u][v] = 1;
			adj[v][u] = 1;
		}
	}

	// Zero the diagonal (distance to self)
	for (std::size_t i = 0; i < adj.size(); i++) {
		adj[i][i] = 0;
	}

	return adj;
}

// Returns a matrix with the shortest paths between all valves
matrix build_shortest_path_matrix() {
	matrix m = build_adjacency_matrix();

	for (int k = 0; k < N; k++)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				m[i][j] = std::min<T>(m[i][j], m[i][k] + m[k][j]);

	return m;
}

int find_best_pressure(matrix const& shortest_paths, bitset const closed_valves, int const valve, int const time) {
	// Pressure accumulator
	int pressure = 0;

	// Iterate the indices in the bitset
	kg::iterate_set(closed_valves, [&](int const n_valve) {
		// Calculate the time needed to move to this neighbouring valve,
		// -1 for the time to open the valve.
		int const n_time = time - 1 - shortest_paths[valve][n_valve];

		// Check that the time limit is not crossed
		if (n_time < 0)
			return;

		// Prepare a bitset with the newly opened valve set.
		// This is used to 'open' the bit in the closed valve set
		bitset const open_n_valve = bitset{1} << n_valve;

		// Calculate the pressure of the remaining valves in the remaining time
		int const rec_pressure = find_best_pressure(shortest_paths, closed_valves ^ open_n_valve, n_valve, n_time);

		// Calculate the total pressure this valve will
		// produce in the remaining minutes it is open
		int const n_pressure = input[n_valve].flow_rate * n_time;

		// Update the maximum pressure found
		pressure = std::max(pressure, n_pressure + rec_pressure);
	});

	return pressure;
}

int main() {
	// Create the shortest-path minute-cost matrix.
	// This holds the time taken to move between valves, without opening them
	auto const shortest_paths = build_shortest_path_matrix();

	// The 'interresting' valves
	bitset valves;
	for (std::size_t i = 0; i < N; i++) {
		if (input[i].flow_rate > 0)
			valves.set(i);
	}

	// Find start position
	auto const aa_pos = std::ranges::find(input, "AA", &valve_input::name);
	int const start_pos = std::ranges::distance(input.begin(), aa_pos);

	// Part 1
	int const max_pressure_30 = find_best_pressure(shortest_paths, valves, start_pos, 30);
	std::cout << "Part 1: " << max_pressure_30 << '\n';

	// Part 2
	std::cout << "Part 2: " << 0 << '\n';
}
