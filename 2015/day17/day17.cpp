#include <iostream>
#include <array>
#include <algorithm>
#include <numeric>
#include <set>

std::array<int, 20> containers{
	33, 14, 18, 20, 45,
	35, 16, 35, 1,  13,
	18, 13, 50, 44, 48,
	6,  24, 41, 30, 42 };
constexpr int Target = 150;

using iterator = decltype(containers)::const_iterator;

int find_combos(int const total, iterator it, iterator const end) {
	if (total  < 0) return 0;
	if (total == 0) return 1;

	int combos = 0;
	while (it != end) {
		combos += find_combos(total - *it, it + 1, end);
		it++;
	}
	return combos;
}

void find_min_containers(int const total, int & min_size, int const depth, iterator it, iterator const end) {
	if (total < 0)
		return;
	if (total == 0) {
		min_size = std::min(depth, min_size);
		return;
	}

	while (it != end) {
		find_min_containers(total - *it, min_size, depth + 1, it + 1, end);
		it++;
	}
}

int find_combos_constrained(int const total, int const MaxContainers, int const containers_used, iterator it, iterator const end) {
	if (total == 0 && containers_used == MaxContainers) return 1;
	if (total < 0  || containers_used > MaxContainers) return 0;

	int combos = 0;
	while (it != end) {
		combos += find_combos_constrained(total - *it, MaxContainers, containers_used + 1, it + 1, end);
		it++;
	}
	return combos;
}

int main() {
	int const total = find_combos(Target, containers.begin(), containers.end());
	std::cout << "Number of container combinations: " << total << '\n';

	int min_size = std::numeric_limits<int>::max();
	find_min_containers(Target, min_size, 0, containers.begin(), containers.end());
	std::cout << "Minimum number of used containers: " << min_size << '\n';

	int const constrained_total = find_combos_constrained(Target, min_size, 0, containers.begin(), containers.end());
	std::cout << "Number of combinations with minimum container use: " << constrained_total << '\n';
}
