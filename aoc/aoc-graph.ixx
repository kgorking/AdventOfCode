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

// Given an adjacency matrix, returns a new matrix with the shortest paths between all nodes.
// m[i][j]==1 if the vertex i is connected with j  by an edge, and 0 if they are not connected by an edge
template <typename T, int N>
	requires(N > 0)
constexpr matrix_t<T, N, N> graph_shortest_path(matrix_t<T, N, N> m) {
	return mat_mul_op(m, m, mat_ops::min);
}


}

