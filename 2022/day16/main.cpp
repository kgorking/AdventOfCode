import aoc;

struct valve_input {
	std::string_view name;
	int flow_rate;
	std::vector<std::string_view> connections;
};

static const auto input = std::to_array<valve_input>({
#if 0
#include "input.txt"
#else
//#include "example.txt"
// #include "testcase1.txt"
// #include "testcase2.txt"
 #include "testcase3.txt"
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

// Part 1: finds the best achievable pressure release
int find_best_pressure(matrix const& shortest_paths, bitset const closed_valves, int const valve, int const time) {
	// Pressure accumulator
	int pressure = 0;

	// Iterate the indices in the bit set
	kg::bit_iterate_set(closed_valves, [&](int const n_valve) {
		// Calculate the time needed to move to this neighboring valve,
		// -1 for the time to open the valve.
		int const n_time = time - 1 - shortest_paths[valve][n_valve];

		// Check that the time limit is not crossed
		if (n_time <= 0)
			return;

		// Prepare a bit set with the newly opened valve set.
		// This is used to 'open' the bit in the closed valve set
		bitset const open_n_valve = bitset{1} << n_valve;

		// Calculate the total pressure this valve will
		// produce in the remaining minutes it is open
		int n_pressure = input[n_valve].flow_rate * n_time;

		// Calculate the pressure of the remaining valves in the remaining time
		if (n_time > 2)
			n_pressure += find_best_pressure(shortest_paths, closed_valves ^ open_n_valve, n_valve, n_time);

		// Update the maximum pressure found
		pressure = std::max(pressure, n_pressure);
	});

	return pressure;
}

// Part 2
int find_best_pressure_p2(matrix const& shortest_paths, bitset const closed_valves, int const valve, int const time) {
	// Pressure accumulator
	std::atomic_int max_pressure{0};

	// Split the work in two
	// TODO could be faster, lambda is hit ~6k times
	auto const half_bits = closed_valves.count() / 2;
	kg::bit_subsets_n_par(closed_valves, half_bits, [&](bitset const some_valves) {
		auto const rest_valves = some_valves ^ closed_valves;

		int const pressure =
			find_best_pressure(shortest_paths, some_valves, valve, time) + find_best_pressure(shortest_paths, rest_valves, valve, time);

		//if (pressure > max_pressure)
		//	max_pressure = pressure;
		int max_p = max_pressure;
		while (pressure > max_p && !max_pressure.compare_exchange_weak(max_p, pressure))
			;
	});

	return max_pressure;
}

int main() {
	// Create the shortest-path minute-cost matrix.
	// This holds the time taken to move between valves, without opening them
	auto const shortest_paths = kg::graph_shortest_path(build_adjacency_matrix());

	// The 'interesting' valves
	bitset valves;
	for (std::size_t i = 0; i < N; i++) {
		if (input[i].flow_rate > 0) {
			valves.set(i);
		}
	}

	// Find start position
	auto const aa_pos = std::ranges::find(input, "AA", &valve_input::name);
	int const start_pos = static_cast<int>(std::distance(input.begin(), aa_pos));

	// Part 1
	int const max_pressure_30 = find_best_pressure(shortest_paths, valves, start_pos, 30);
	std::cout << "Part 1: " << max_pressure_30 << '\n';

	// Part 2
	for (int i = 0; i < 100; i += 1) {
		int max_pressure_26 = find_best_pressure_p2(shortest_paths, valves, start_pos, 26);
		std::cout << "Part 2: " << max_pressure_26 << '\n';
	}
}
