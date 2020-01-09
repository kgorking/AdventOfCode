#include <iostream>
#include <set>
#include "input.h"

int main()
{
	using location = std::pair<int, int>;

	location current_loc{ 0,0 };
	std::set<location> houses_visited{ current_loc };

	for (char const direction : input) {
		switch (direction) {
			case '^': current_loc.second++; break;
			case 'v': current_loc.second--; break;
			case '>': current_loc.first++; break;
			case '<': current_loc.first--; break;
			case 0: break;
			default: return -1;
		}

		if (houses_visited.count(current_loc) == 0)
			houses_visited.emplace(current_loc);
	}

	std::cout << houses_visited.size() << " houses visited\n";

	// part 2
	houses_visited.clear();
	houses_visited.emplace(location{ 0,0 });
	location current_locs[2]{ {0, 0}, {0, 0} };
	bool santa_or_robo = true;

	for (char const direction : input) {
		switch (direction) {
			case '^': current_locs[santa_or_robo].second++; break;
			case 'v': current_locs[santa_or_robo].second--; break;
			case '>': current_locs[santa_or_robo].first++; break;
			case '<': current_locs[santa_or_robo].first--; break;
			case 0: break;
			default: return -1;
		}

		if (houses_visited.count(current_locs[santa_or_robo]) == 0)
			houses_visited.emplace(current_locs[santa_or_robo]);

		// Switch turns
		santa_or_robo = !santa_or_robo;
	}
	std::cout << houses_visited.size() << " houses visited\n";
}
