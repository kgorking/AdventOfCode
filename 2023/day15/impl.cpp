import aoc;

struct hash {
	constexpr unsigned char operator()(std::string_view sv) const {
		unsigned char value = 0;
		for (char c : sv) {
			value += c;
			value *= 17;
			value %= 256;
		}
		return value;
	}
};
static_assert(52 == hash{}("HASH"));

constexpr auto part1(auto const& input) {
	return std::transform_reduce(input.begin(), input.end(), 0, std::plus<>{}, hash{});
}

constexpr auto part2(auto const& input) {
	auto boxes = std::unordered_map<std::string_view, unsigned char, hash>{};

	for (auto op : input) {
		if (op.back() == '-') {
			op.remove_suffix(1);
			boxes.erase(op);
		} else { // '=X'
			int const val = op.back() - '0';
			op.remove_suffix(2);
			boxes[op] = val;
		}
	}

	int focusing_power = 0;
	for (int i = 0; i < boxes.bucket_count(); i++) {
		if (boxes.bucket_size(i)) {
			int lens_slot = boxes.bucket_size(i);
			std::for_each(boxes.begin(i), boxes.end(i), [&](auto const pair) {
				focusing_power += (1 + i) * lens_slot-- * pair.second;
			});
		}
	}

	return focusing_power;
}

auto solve(auto const& input) {
	return std::make_pair(part1(input), part2(input));
}

constexpr auto expected_sample = std::make_pair(1320, 145);
constexpr auto expected_input = std::make_pair(514639, 279470);
constexpr auto sample_input = std::to_array<std::string_view>({
#include "sample_input.txt"
});
constexpr auto actual_input = std::to_array<std::string_view>({
#include "input.txt"
});
