#include <iostream>
#include <array>
#include "input.h"

using guestlist = std::array<guest, 9>;

int calc_happiness(guestlist const& gl) {
	int happ = 0;
	for (size_t i = 0; i < gl.size() - 1; i++) {
		happ += happiness[{gl[i], gl[i + 1]}];
		happ += happiness[{gl[i + 1], gl[i]}];
	}

	happ += happiness[{gl[0], gl[gl.size() - 1]}];
	happ += happiness[{gl[gl.size() - 1], gl[0]}];

	return happ;
}

int main() {
	 guestlist gl{
		guest::Alice,
		guest::Bob,
		guest::Carol,
		guest::David,
		guest::Eric,
		guest::Frank,
		guest::George,
		guest::Mallory,
		guest::Myself
	};

	 int max_happ = 0;
	 do {
		 int happ = calc_happiness(gl);
		 max_happ = std::max(happ, max_happ);
	 } while (std::next_permutation(gl.begin(), gl.end()));

	 std::cout << max_happ << '\n';
}
