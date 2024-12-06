export module aoc:ranges;
import std;

export namespace kg {
export auto lookup(auto&& v, int I, auto... Is) {
	if constexpr (sizeof...(Is) > 0) {
		return lookup(v[I], Is...);
	} else
		return v[I];
}

// Index projection
template<int ...Is>
struct index {
	constexpr auto operator()(auto&& val) const {
		if constexpr (0 == sizeof...(Is))
			return val;
		else {
			return lookup(val, Is...);
		}
	}
};

constexpr auto sum = std::bind_back(std::ranges::fold_left, 0ll, std::plus {});
}
