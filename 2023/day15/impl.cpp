import aoc;
using u8 = unsigned char;

struct hash {
	constexpr u8 operator()(std::string_view sv) const {
		return std::ranges::fold_left(sv, u8{0}, [](u8 acc, u8 c) { return (acc + c) * 17; }); // %256 is implicit
	}
};

constexpr auto part1(auto const& input) { // array of std::string_view
	return std::transform_reduce(input.begin(), input.end(), 0, std::plus<>{}, hash{});
}

auto part2(auto const& input) { // array of std::string_view
	auto boxes = std::unordered_map<std::string_view, u8, hash>{};

	// Build hash map
	for (std::string_view op : input) {
		if (op.back() == '-') {
			op.remove_suffix(1);
			boxes.erase(op);
		} else { // '=X'
			u8 const val = op.back() - '0';
			op.remove_suffix(2);
			boxes[op] = val;
		}
	}

	// Calculate focusing power
	int focusing_power = 0;
	for (int i = 0; i < boxes.bucket_count(); i++)
		if (int lens_slot = boxes.bucket_size(i); lens_slot > 0)
			std::for_each(boxes.begin(i), boxes.end(i), [&](auto const pair) { focusing_power += (1 + i) * lens_slot-- * pair.second; });

	return focusing_power;
}

auto solve(auto const& input) {
	return std::make_pair(part1(input), part2(input));
}