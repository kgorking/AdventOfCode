#include <iostream>
#include <array>
#include <memory>
#include <algorithm>

constexpr int table_size = 1'000'000;
using table = std::array<int, table_size>;
using utable = std::unique_ptr<table>;

template <int presents_per_house>
utable calc_presents() {
	auto presents = std::make_unique<table>();
	presents->fill(1);
	auto div_ptr = presents->data();
	for (int i = 2; i < table_size; i++) {
		for (int j = i, house_counter=0; house_counter < 50 && j < table_size; j += i, house_counter++) {
			div_ptr[j] += i;
		}
	}

	std::for_each(presents->begin(), presents->end(), [](int& presents) {
		presents *= presents_per_house;
	});

	return presents;
}

int main() {
	constexpr int input = 29'000'000;

	// Build the lookup table
	auto const presents = calc_presents<11>();

	// Find the first house that get at least 'input' presents
	auto const it = std::find_if(presents->begin(), presents->end(), [](int presents) {
		return presents >= input;
	});

	// Print it
	if (it == presents->end())
		std::cout << '\'' << input << "' not found. Max value is " << presents->back() << '\n';
	else
		std::cout << "House nr. " << std::distance(presents->begin(), it) << " got " << *it << " presents.\n";
}
