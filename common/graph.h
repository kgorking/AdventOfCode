export namespace kg {


// Given an adjacency matrix, returns a new matrix with the shortest paths between all nodes
template <typename T, int N>
	requires(N > 0)
constexpr matrix_t<T, N, N> graph_shortest_path(matrix_t<T, N, N> adj) {
	for (int k = 0; k < N; k++)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				adj[i][j] = std::min<T>(adj[i][j], adj[i][k] + adj[k][j]);

	return adj;
}

}

