export module aoc:visited;
import std;
import :pos2d;

struct size_info {
	int w, h;
};

template <int Columns = -1, int Rows = -1>
struct empty_size_info {
	static constexpr int w = Rows;
	static constexpr int h = Columns;
};

export namespace kg {
template <int Columns = -1, int Rows = -1>
	requires((Rows == -1 || Rows > 0) && (Columns == -1 || Columns > 0))
class visited : std::conditional_t<(Columns < 1 || Rows < 1), size_info, empty_size_info<Columns, Rows>> {
	using value_type = std::conditional_t< Columns < 0 || Rows<0, std::vector<bool>, std::bitset<Rows * Columns>>;

	value_type data;

public:
	constexpr visited()
		requires(Rows > 0 && Columns > 0)
	{
	}

	constexpr visited(int columns, int rows)
		requires(Rows == -1 || Columns == -1)
	{
		this->w = rows;
		this->h = columns;
		if (rows < 1 || columns < 1)
			throw std::invalid_argument("Invalid arguments for rows/columns");
		data.resize(columns * rows);
	}

	constexpr void set(kg::pos2di p) {
		data[p.y * this->w + p.x] = 1;
	}

	constexpr bool test(kg::pos2di p) const {
		return 1 == data[p.y * this->w + p.x];
	}

	constexpr bool test_or_set(kg::pos2di p) {
		if (!test(p)) {
			set(p);
			return false;
		} else {
			return true;
		}
	}
};
}
