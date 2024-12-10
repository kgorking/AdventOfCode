export module aoc.day09;
import aoc;
import std;

// Holds the expected values for sample + input. Used in unit tests.
export constexpr auto expected_sample = std::make_pair(1928, 2858);
export constexpr auto expected_input = std::make_pair(6463499258318ll, 6493634986625ll);

struct block {
	int id, size, pos;
};

auto convert_to_blocks(std::string_view s) {
	std::vector<block> files, spaces;

	bool is_file = true;
	for (int id = 0, pos = 0; auto c : s) {
		int const size = c - '0';

		if (is_file) {
			// add file
			files.emplace_back(id++, size, pos);
		} else {
			// add space
			spaces.emplace_back(-1, size, pos);
		}
		pos += size;

		is_file = !is_file;
	}

	return std::pair { files, spaces };
}

export auto part1(auto&& input) {
	auto [files, spaces] = convert_to_blocks(input);

	// "Defrag"
	std::vector<block> defragged;
	int ri = files.size() - 1;

	for (auto [i, free] : spaces | std::views::enumerate) {
		if (files[i].size > 0)
			defragged.push_back(files[i]);

		if (i >= ri)
			break;

		while (free.size > 0) {
			int const take = std::min(free.size, files[ri].size);
			files[ri].size -= take;
			free.size -= take;

			if (take > 0)
				defragged.emplace_back(files[ri].id, take, 0);

			if (files[ri].size == 0)
				ri--;
		}
	}

	// Calc checksum
	std::int64_t checksum = 0;
	for (int idx = 0; block b : defragged) {
		for (int i = 0; i < b.size; i++) {
			checksum += std::int64_t { b.id } * idx++;
		}
	}
	return checksum;
}

export auto part2(auto&& input) {
	auto [files, spaces] = convert_to_blocks(input);

	using cmp = decltype([](block l, block r) { return l.pos > r.pos; });
	using heap = std::priority_queue<block, std::vector<block>, cmp>;

	// Heaps for files and empty spaces
	std::array<heap, 10> heaps;
	heap& file_heap = heaps[0];

	// Load up the empty spaces
	for (block space : spaces) {
		if (space.size > 0)
			heaps[space.size].push(space);
	}

	// "Defrag"
	while (!files.empty()) {
		block file = files.back();
		files.pop_back();

		if (file.size == 0)
			continue;

		auto valid_heaps = heaps
						 // skip spaces smaller than the file
						 | std::views::drop(file.size)
						 // skip empty heaps
						 | std::views::filter(std::not_fn(&heap::empty));

		auto it = std::ranges::min_element(valid_heaps, [](heap const& l, heap const& r) { return l.top().pos < r.top().pos; });
		if (it != valid_heaps.end()) {
			heap& free_heap = *it;

			if (file.pos > free_heap.top().pos) {
				block free = free_heap.top();
				free_heap.pop();

				// add new free space at file pos
				heaps[file.size].push({-1, file.size, file.pos});

				file.pos = free.pos;

				// shrink free space
				free.size -= file.size;
				free.pos += file.size;

				if (free.size > 0)
					heaps[free.size].push(free);
			}
		}

		file_heap.push(file);
	}

	// Calc checksum
	std::int64_t checksum = 0;
	int idx = 0;
	while (true) {
		auto valid_heaps = heaps | std::views::filter(std::not_fn(&heap::empty));
		auto it = std::ranges::min_element(valid_heaps, [](heap const& l, heap const& r) { return l.top().pos < r.top().pos; });
		if (it == valid_heaps.end())
			break;

		block b = it->top();
		it->pop();

		if (b.id == -1) {
			idx += b.size;
		} else {
			for (int i = 0; i < b.size; i++) {
				checksum += std::int64_t { b.id } * idx++;
			}
		}
	}

	return checksum;
}
