#pragma once

namespace kg {

// A helper class for creating a spanning tree.
// Insert the edges (a,b) in sorted order.
// Use:
//  for (...) {
//	 if (!same(a, b))
//	  unite(a, b);
//  }
//

template <typename T>
class min_spanning_tree {
	std::vector<T> link;
	std::vector<T> size;

public:
	explicit min_spanning_tree(std::size_t vertices) : link(vertices), size(vertices) {
		std::ranges::iota(link, 0);
		std::ranges::fill(size, 1);
	}

	T find(T x) const {
		while (x != link[x])
			x = link[x];
		return x;
	};
	
	bool same(T a, T b) const {
		return find(a) == find(b);
	};

	void unite(T a, T b) {
		a = find(a);
		b = find(b);
		if (size[a] < size[b])
			std::swap(a, b);
		size[a] += size[b];
		link[b] = a;
	};
};
} // namespace kg