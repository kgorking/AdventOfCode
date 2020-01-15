#include <iostream>
#include <tuple>
#include <array>

struct reindeer {
	int speed;
	int fly_time;
	int rest;
};

std::array<reindeer, 9> const input{ reindeer
	{19, 7, 124},
	{3, 15, 28},
	{19, 9, 164},
	{19, 9, 158},
	{13, 7, 82},
	{25, 6, 145},
	{14, 3, 38},
	{3, 16, 37},
	{25, 6, 143}
};

int calc_distance(reindeer const r, int time) {
	//return r.speed * r.fly_time * (time / (r.fly_time + r.rest) + 1); // almost there, but not quite

	int dist = 0;
	while (true) {
		if (time - r.fly_time <= 0)
			return dist + time * r.speed;
		time -= r.fly_time;
		dist += r.speed * r.fly_time;

		if (time - r.rest <= 0)
			return dist;
		time -= r.rest;
	}
}

int main() {
	int max_dist = 0;
	for (reindeer d : input) {
		max_dist = std::max(max_dist, calc_distance(d, 2503));
	}

	std::cout << "distance of the winning reindeer: " << max_dist << '\n';

	std::array<int, 9> points{};
	for (int i = 1; i <= 2503; i++) {
		std::array<int, 9> dist;
		for (size_t j = 0; j < 9; j++)
			dist[j] = calc_distance(input[j], i);

		auto const max = std::max_element(dist.begin(), dist.end());
		for (size_t j = 0; j < 9; j++)
			if (dist[j] == *max)
				points[j]++;
	}

	auto const max = std::max_element(points.begin(), points.end());
	std::cout << "Max points: " << *max << '\n';
}
