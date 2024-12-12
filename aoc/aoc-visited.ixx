export module aoc:visited;
import std;
import :pos2d;

export namespace kg {
template <int Columns = -1, int Rows = -1>
	requires((Rows == -1 || Rows > 0 ) && (Columns == -1 || Columns > 0))
class visited {
	using row_type = std::conditional_t<Rows < 0, std::vector<bool>, std::bitset<Rows>>;
	using value_type = std::conditional_t<Columns < 0, std::vector<row_type>, std::array<row_type, Columns>>;

	value_type data;

public:
	constexpr visited(int columns = -1, int rows = -1) {
		if constexpr (Columns == -1) {
			if (columns == -1)
				throw std::invalid_argument("-1 is not a valid value for 'columns'");
			data.resize(columns);
		} else {
			data.fill({});
		}

		if constexpr (Rows == -1) {
			if (rows == -1)
				throw std::invalid_argument("-1 is not a valid value for 'rows'");
			for (auto& r : data)
				r.resize(rows);
		}
	}

	constexpr void set(kg::pos2di p) {
		data[p.y][p.x] = 1;
	}

	constexpr bool test(kg::pos2di p) const {
		return 1 == data[p.y][p.x];
	}

	constexpr bool test_or_set(kg::pos2di p) {
		if(!test(p)) {
			set(p);
			return false;
		} else {
			return true;
		}
	}
};
}
