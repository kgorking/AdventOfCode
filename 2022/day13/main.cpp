import std;

struct item {
	std::size_t first = 0, last = 0;
	std::string_view source;
	std::vector<item> items;

	constexpr bool operator==(item const& other) const noexcept {
		return source == other.source;
	}
	constexpr std::weak_ordering operator<=>(item const& i) const noexcept {
		if (!items.empty() && !i.items.empty()) {
			return items <=> i.items;
		}

		if (items.empty() && !i.items.empty()) {
			if (source.empty())
				return std::weak_ordering::less;
			return std::vector<item>{*this} <=> i.items;
		}

		if (!items.empty() && i.items.empty()) {
			if (i.source.empty())
				return std::weak_ordering::greater;
			return items <=> std::vector<item>{i};
		}

		return std::atoi(source.data()) <=> std::atoi(i.source.data());
	}
};

item parse_list_ranges(std::string_view sv) {
	std::size_t index = 0;
	std::stack<item> stack;

	while (index < sv.size()) {
		if (sv[index] == '[') {
			// new list
			index += 1;
			stack.emplace(index);
		} else if (sv[index] == ']') {
			// end list
			item i = stack.top();
			stack.pop();

			i.last = index;
			index += 1;
			i.source = sv.substr(i.first, i.last - i.first);

			if (stack.empty())
				return i;
			else
				stack.top().items.push_back(i);
		} else {
			while (std::isdigit(sv[index])) {
				std::size_t const end = sv.find_first_of(",]", index + 1);
				item i{index, end, sv.substr(index, end - index)};
				stack.top().items.push_back(i);
				index = end;

				if (sv[index] == ',')
					index += 1;
			}
		}

		if (sv[index] == ',')
			index += 1;
	}

	std::unreachable();
}

constexpr auto input = std::to_array<std::string_view>({
#include "input.txt"
});

int main() {
	// Part 1
	int sum = 0;
	std::vector<item> packets;
	for (int i = 0; i < input.size(); i+=3) {
		item const left = parse_list_ranges(input[i]);
		item const right = parse_list_ranges(input[i+1]);
		packets.push_back(left);
		packets.push_back(right);

		auto const cmp = (left < right);
		sum += cmp * (1+i/3);
	}

	std::cout << "Part 1: " << sum << '\n';

	// Part 2
	item const divider2 = parse_list_ranges("[[2]]");
	item const divider6 = parse_list_ranges("[[6]]");
	packets.push_back(divider2);
	packets.push_back(divider6);

	std::sort(packets.begin(), packets.end());

	auto const it2 = std::find(packets.begin(), packets.end(), divider2);
	auto const it6 = std::find(packets.begin(), packets.end(), divider6);
	auto const index2 = 1 + std::distance(packets.begin(), it2);
	auto const index6 = 1 + std::distance(packets.begin(), it6);

	std::cout << "Part 2: " << index2 * index6 << '\n';
}
