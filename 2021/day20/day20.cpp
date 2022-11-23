#include <array>
#include <iostream>
#include <string_view>
#include <vector>

// TODO: not done
// Start from center off grid
// Fill grid with alternating on/off patterns to remove need for bounds checking

struct puzzle_input {
	std::string_view algorithm;
	std::string_view image;
};

constexpr puzzle_input input{
	#include "input.txt"
};

constexpr int image_dim = 100;

union pixel {
	struct {
		bool val : 4;
		bool old : 4;
	} parts;
	bool whole;
};

struct grid {
	grid(int dim) : data(dim * dim), dim(dim) {}

	void set_initial_val(int x, int y) {
		data[y * dim + x].parts.old = true;
	}

	// Returns value at x,y from previous step
	bool get_old(int x, int y, int step) const {
		return data[y * dim + x].parts.old;
	}

	void set_new(int x, int y, int step) {
		data[y * dim + x].parts.val = true;
	}

	bool in_bounds(int x, int y, int step) const {
		int const stride = y * (image_dim + 2 * (step-1));
		return (x >= 0) && (x < stride) && (y >= 0) && (y < stride);
	}

	void shift_new_to_old() {
		for (pixel& p : data) {
			p.parts.old = p.parts.val;
			p.parts.val = false;
		}
	}

	size_t count_lit() const {
		size_t count = 0;
		for (pixel p : data) {
			count += p.parts.val;
		}
		return count;
	}

	std::vector<pixel> data;
	int dim;
};

constexpr bool is_input_pixel_lit(int x, int y) {
	return '#' == input.image.at(1 + y * (image_dim + 1) + x); // +1 for the newlines
}

bool lookup_pixel(grid const& g, int x, int y, int step, char const oob_value) {
	if (!g.in_bounds(x, y, step)) {
		return '#' == oob_value;
	}
	else {
		return g.get_old(x, y, step);
	}
}

void transform_step(grid& pixels, int const step, char const oob_value) {
	for (int y = 0; y < image_dim + 2*step; ++y) {
		for (int x = 0; x < image_dim + 2*step; ++x) {
			int result = 0;

			result |= lookup_pixel(pixels, x - 1, y - 1, step, oob_value) << 8;
			result |= lookup_pixel(pixels, x,     y - 1, step, oob_value) << 7;
			result |= lookup_pixel(pixels, x + 1, y - 1, step, oob_value) << 6;

			result |= lookup_pixel(pixels, x - 1, y, step, oob_value) << 5;
			result |= lookup_pixel(pixels, x,     y, step, oob_value) << 4;
			result |= lookup_pixel(pixels, x + 1, y, step, oob_value) << 3;

			result |= lookup_pixel(pixels, x - 1, y + 1, step, oob_value) << 2;
			result |= lookup_pixel(pixels, x,     y + 1, step, oob_value) << 1;
			result |= lookup_pixel(pixels, x + 1, y + 1, step, oob_value) << 0;

			if ('#' == input.algorithm[result]) {
				pixels.set_new(x, y, step);
			}
		}
	}
}

grid create_initial_storage(int steps_needed) {
	int const dimension = 1 + 2 * steps_needed + image_dim;
	grid pixels(dimension);

	for (int y = 0; y < image_dim; ++y) {
		for (int x = 0; x < image_dim; ++x) {
			if (is_input_pixel_lit(x, y)) {
				pixels.set_initial_val(x, y);
			}
		}
	}

	return pixels;
}

int main() {
	// Convert the initial image to a set of pixels
	grid g = create_initial_storage(50);

	// All out-of-bounds pixels are dark in the beginning
	char oob_value = '.';

	for (int step = 1; step <= 50; ++step) {
		transform_step(g, step, oob_value);

		// Update the out-of-bounds value
		oob_value = (oob_value == '#') ? input.algorithm.back() : input.algorithm.front();

		std::cout << "Step " << step << ": " << g.count_lit() << '\n'; // 5437

		g.shift_new_to_old();
	}
}
