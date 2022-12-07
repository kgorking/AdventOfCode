#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>

static constexpr std::string_view input{
#include "input.txt"
};

int build_dirs(std::string_view& sv, std::vector<int>& sizes) {
	int current_dir_size = 0;

	// Check for command
	while (!sv.empty()) {
		if (sv[0] == '$') {
			if (sv[2] == 'c') { // CD
				// Check if moving out of directory
				bool const move_out = sv[5] == '.';

				// skip to next line
				sv.remove_prefix(1 + sv.find('\n'));

				if (move_out) {
					return current_dir_size;
				} else {
					// Switch to new directory, save its resulting size
					// and accumulate it into the current directory size.
					int const size = build_dirs(sv, sizes);
					sizes.push_back(size);
					current_dir_size += size;
				}
			} else if (sv[2] == 'l') { // LS
				sv.remove_prefix(5);   // skip '$ ls\n'

				// Accumulate file sizes
				while (!sv.empty() && sv[0] != '$') {
					// Ignore 'dir'
					if (sv[0] != 'd') {
						current_dir_size += std::atoi(sv.data());
					}

					// skip to next line
					sv.remove_prefix(1 + sv.find('\n'));
				}
			}
		}
	}

	return current_dir_size;
}

int main() {
	std::vector<int> directory_sizes;
	std::string_view input_copy = input;
	build_dirs(input_copy, directory_sizes);

	// Sort the directory sizes
	std::ranges::sort(directory_sizes);

	int64_t total_under_100k = 0;
	for (auto const size : directory_sizes) { // std::accumulate_if when?
		if (size >= 100'000)
			break;
		total_under_100k += size;
	}
	std::cout << "Part 1: " << total_under_100k << '\n';

	int const total_space = 70'000'000;
	int const needed_space = 30'000'000;
	int const used_space = directory_sizes.back();
	int const free_space = total_space - used_space;
	int const missing_space = needed_space - free_space;

	auto const it = std::ranges::upper_bound(directory_sizes, missing_space);
	std::cout << "Part 2: " << *it << '\n';
}
