#include <iostream>
#include <numeric>
#include <array>
#include <vector>

std::array<int, 28> constexpr package_weights{
	1, 3, 5, 11, 13, 17, 19, 23, 29, 31,
	37, 41, 43, 47, 53, 59, 67, 71, 73,
	79, 83, 89, 97, 101, 103, 107, 109, 113 };

using quantum_entanglement = std::pair<size_t, uint64_t>; // num packackes, qe 
quantum_entanglement constexpr bad_entanglement{ std::numeric_limits<int>::max(), std::numeric_limits<uint64_t>::max() };

// modified from day17 'find_min_containers'
template <class It>
void find_min_packages(int const remaining_weight, std::vector<int>& stack, quantum_entanglement & min_qe, It it, It const end) {
	if (remaining_weight == 0) {
		auto const qe = std::reduce(stack.begin(), stack.end(), uint64_t{ 1 }, std::multiplies<>{});
		if (stack.size() < min_qe.first) {
			min_qe = { stack.size(), qe };
			for (int val : stack)
				std::cout << val << ' ';
			std::cout << ": " << qe << '\n';
		}
		else if (stack.size() == min_qe.first) {
			if (qe < min_qe.second) {
				min_qe = { stack.size(), qe };
				for (int val : stack)
					std::cout << val << ' ';
				std::cout << ": " << qe << '\n';
			}
		}
		return;
	}

	// Bust out early if
	//   weight has been exceeded
	//   I'm at the same stack level as the current best result
	//   I have reached the end of the package weights
	if (remaining_weight < 0 || stack.size() == min_qe.first || it == end) {
		return;
	}

	while (it != end) {
		stack.push_back(*it);
		find_min_packages(remaining_weight - *it, stack, min_qe, it + 1, end);
		stack.pop_back();
		it++;
	}
}


int main() {
	int const total_weight = std::reduce(package_weights.begin(), package_weights.end(), 0);
//	int const weight_per_group = total_weight / 3; // part 1
	int const weight_per_group = total_weight / 4; // part 2

	std::vector<int> stack;
	stack.reserve(package_weights.size());
	quantum_entanglement min_qe{ bad_entanglement };
	find_min_packages(weight_per_group, stack, min_qe, package_weights.rbegin(), package_weights.rend());
	std::cout << "packages: " << min_qe.first << "\nquantum entanglement: " << min_qe.second << '\n';
}
