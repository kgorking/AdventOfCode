export module aoc:visited;
import std;
import :pos2d;
import :grid;

struct size_info {
	int w, h;
};

template <int Columns = -1, int Rows = -1>
struct empty_size_info {
	static constexpr int w = Rows;
	static constexpr int h = Columns;
};

export namespace kg {
template <typename T = bool, int Columns = -1, int Rows = -1>
	requires((Rows == -1 || Rows > 0) && (Columns == -1 || Columns > 0))
struct visited : std::conditional_t<(Columns < 1 || Rows < 1), size_info, empty_size_info<Columns, Rows>> {
	static constexpr bool use_bitset = std::same_as<bool, T> && Columns >= 1 && Rows >= 1;
	using value_type = std::conditional_t<use_bitset, std::bitset<Rows * Columns>, std::vector<T>>;

	constexpr visited() requires(use_bitset) = default;
	constexpr visited() requires(!use_bitset) = delete;

	constexpr visited(int columns, int rows)
		requires(!use_bitset)
	{
		this->w = rows;
		this->h = columns;
		if (rows < 1 || columns < 1)
			throw std::invalid_argument("Invalid arguments for rows/columns");
		data.resize(columns * rows);
	}

	template<typename U>
	constexpr visited(grid<U> const& g, T initial_value = T {}) {
		if constexpr (!use_bitset) {
			this->w = g.width();
			this->h = g.height();
			data.resize(this->w * this->h, initial_value);
		} else {
			if (this->w != g.width() || this->h != g.height())
				throw std::invalid_argument("size mismatch");
		}
	}

	constexpr T& operator[](kg::pos2di p) {
		return data[p.y * this->w + p.x];
	}

	constexpr T operator[](kg::pos2di p) const {
		return data[p.y * this->w + p.x];
	}

	constexpr void set(kg::pos2di p, T val = 1) {
		data[p.y * this->w + p.x] = val;
	}

	constexpr bool test(kg::pos2di p, T val = 1) const {
		return val == data[p.y * this->w + p.x];
	}

	constexpr bool test_or_set(kg::pos2di p, T val = 1) {
		if (!test(p, val)) {
			set(p, val);
			return false;
		} else {
			return true;
		}
	}

	private:
	value_type data;
};
}
