import common;

struct pair {
	kg::range<char> r1, r2;

	constexpr bool one_contains_other() const { return r1.contains(r2) || r2.contains(r1); }
	constexpr bool one_overlaps_other() const { return r1.overlaps(r2) || r2.overlaps(r1); }
};

static constexpr auto input = std::to_array<pair>({
	#include "input.txt"
});

int main() {
	// Part 1
	static constexpr int num_contained = std::accumulate(input.begin(), input.end(), 0, [](int val, pair const& p) {
		return val + p.one_contains_other();
	});
	std::cout << num_contained << '\n';

	// Part 2
	static constexpr int num_overlapped = std::accumulate(input.begin(), input.end(), 0, [](int val, pair const& p) {
		return val + p.one_overlaps_other();
	});
	std::cout << num_overlapped << '\n';
}
