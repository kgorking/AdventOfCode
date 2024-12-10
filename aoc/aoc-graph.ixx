export module aoc : graph;
import :matrix;
import :pos2d;

export namespace kg {

// Note that adjacency matrices are nodes_count^2
template <typename T, int R, int C>
auto make_adjacency_mat(kg::matrix_t<T, R, C> mat, auto&& bin_op) -> std::unique_ptr<kg::matrix_t<T, R * C>> {
	constexpr kg::pos2di offsets[] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	auto adj = std::make_unique<kg::matrix_t<T, R * C>>();
	kg::mat_max(*adj);

	auto valid = [](kg::pos2di p) { return p.x >= 0 && p.y >= 0 && p.x < R && p.y < C; };

	for (int j = 0; j < C; j++) {
		for (int i = 0; i < R; i++) {
			kg::pos2di c { i, j };
			int const c_i = j * R + i;

			for (auto offset : offsets) {
				kg::pos2di const n = c + offset;
				if (!valid(n))
					continue;

				int const n_i = n.y * R + n.x;
				if (bin_op(mat[c.y][c.x], mat[n.y][n.x])) {
					(*adj)[c_i][n_i] = 1;
				}
			}
		}
	}

	return adj;
}

// Travers all nodes and find all paths until an end condition is met.
// The use of callbacks allows for state injection by the user, fx. a
// 'visited' state can be maintained in 'is_at_end'
void traverse_all_paths(auto const& terrain, kg::pos2di const start_pos, auto&& is_at_end, auto&& valid_neighbour) {
	const size_t height = terrain.size();
	const size_t width = terrain[0].size();

	// Initialize queue
	std::vector<kg::pos2di> queue;
	queue.push_back(start_pos);

	while (!queue.empty()) {
		auto const p = queue.back();
		queue.pop_back();

		if (is_at_end(p, terrain[p.y][p.x])) {
			continue;
		}

		for (auto const dir : std::to_array<kg::pos2di>({ { 0, -1 }, { 0, +1 }, { -1, 0 }, { +1, 0 } })) {
			// Check that the position is in bounds
			kg::pos2di const adj_pos = p + dir;
			if (adj_pos.x < 0 || adj_pos.x >= width || adj_pos.y < 0 || adj_pos.y >= height)
				continue;

			// Add neighbour to the stack
			int const adj_val = terrain[adj_pos.y][adj_pos.x];
			if (valid_neighbour(terrain[p.y][p.x], adj_val))
				queue.push_back(adj_pos);
		}
	}
}

// Given an adjacency matrix, returns a new matrix with the shortest paths between all nodes.
// m[i][j]==1 if the vertex i is connected with j  by an edge, and 0 if they are not connected by an edge
template <typename T, int N>
	requires(N > 0)
constexpr matrix_t<T, N, N> graph_shortest_path(matrix_t<T, N, N> m) {
	return mat_mul_op(m, m, mat_ops::min);
}


}

