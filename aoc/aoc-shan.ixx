export module aoc:shan;

// TODO
export
namespace kg {
auto L(auto& fn) {
	return [&](auto... args) {
		return fn(args...);
	};
}
// operator *
// operator +
}
