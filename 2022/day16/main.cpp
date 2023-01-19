import common;

struct valve_input {
	std::string_view name;
	int flow_rate;
	std::vector<std::string_view> connections;
};

const auto input = std::to_array<valve_input>({
#if 0
#include "input.txt"
#else
#include "example.txt"
// #include "testcase1.txt"
// #include "testcase2.txt"
// #include "testcase3.txt"
// #include "testcase4.txt"
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

struct result_pair {
	int pressure;
	bitset valves;
};

// Part 1: finds the best achiveable pressure release
int find_best_pressure(matrix const& shortest_paths, bitset const closed_valves, int const valve, int const time) {
	// Pressure accumulator
	int pressure = 0;

	// Iterate the indices in the bitset
	kg::iterate_set(closed_valves, [&](int const n_valve) {
		// Calculate the time needed to move to this neighbouring valve,
		// -1 for the time to open the valve.
		int const n_time = time - 1 - shortest_paths[valve][n_valve];

		// Check that the time limit is not crossed
		if (n_time <= 0)
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

// Part 2: expanded version of part 1, which saves pressure-bitset pairs as well
int find_best_pressure_p2(matrix const& shortest_paths, bitset const closed_valves, int const valve, int const time,
						  std::vector<result_pair>& results) {
	// Pressure accumulator
	int pressure = 0;

	// Iterate the indices in the bitset
	kg::iterate_set(closed_valves, [&](int const n_valve) {
		// Calculate the time needed to move to this neighbouring valve,
		// -1 for the time to open the valve.
		int const n_time = time - 1 - shortest_paths[valve][n_valve];

		// Check that the time limit is not crossed
		if (n_time <= 0)
			return;

		// Open the valve by flipping its bit
		bitset const remaining_valves = closed_valves ^ (bitset{1} << n_valve);

		// Calculate the pressure of the remaining valves in the remaining time
		int const rec_pressure = find_best_pressure_p2(shortest_paths, remaining_valves, n_valve, n_time, results);

		// Calculate the total pressure this valve will
		// produce in the remaining minutes it is open
		int const n_pressure = rec_pressure + input[n_valve].flow_rate * n_time;

		// Update the maximum pressure found
		if (n_pressure > pressure) {
			results.push_back({n_pressure, remaining_valves});
			pressure = n_pressure;
		}
	});

	return pressure;
}

int main() {
	// Create the shortest-path minute-cost matrix.
	// This holds the time taken to move between valves, without opening them
	auto const shortest_paths = kg::graph_shortest_path(build_adjacency_matrix());

	// The 'interresting' valves
	bitset valves;
	for (std::size_t i = 0; i < N; i++) {
		if (input[i].flow_rate > 0) {
			valves.set(i);
		}
	}

	// Find start position
	auto const aa_pos = std::ranges::find(input, "AA", &valve_input::name);
	int const start_pos = static_cast<int>(std::ranges::distance(input.begin(), aa_pos));

	// Part 1
	int const max_pressure_30 = find_best_pressure(shortest_paths, valves, start_pos, 30);
	std::cout << "Part 1: " << max_pressure_30 << '\n';

	// Part 2
	std::vector<result_pair> results;
	find_best_pressure_p2(shortest_paths, valves, start_pos, 26, results);
	//std::ranges::sort(results, [](result_pair const& l, result_pair const& r) {
	//	return l.pressure < r.pressure;
	//});

	std::size_t const num_valves = valves.count();
	int max_pressure_26 = 0;
	for (auto l = results.begin(); l != results.end(); ++l) {
		for (auto r = l + 1; r != results.end(); ++r) {
			bitset const closed_valves = l->valves ^ r->valves;

			if (closed_valves == valves) {
				max_pressure_26 = std::max(max_pressure_26, l->pressure + r->pressure);
			}
		}
	}

	// example = 1707
	// > 1512
	std::cout << "Part 2: " << max_pressure_26 << '\n';
}
