import aoc;

struct hash {
	constexpr int operator()(std::string_view sv) const {
		int value = 0;
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
	using hashmap = std::unordered_map<std::string_view, int, hash>;
	hashmap boxes;

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
	for (int i = 0; i < 256 && i < boxes.bucket_count(); i++) {
		if (0 == boxes.bucket_size(i))
			continue;

		auto first = boxes.begin(i);
		auto const end = boxes.end(i);

		int lens_slot = boxes.bucket_size(i);
		while (first != end) {
			focusing_power += (1 + i) * lens_slot * first->second;
			--lens_slot;
			++first;
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
