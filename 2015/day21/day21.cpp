#include <iostream>
#include <algorithm>
#include <tuple>
#include <array>

using character = std::tuple<int, int, int>;
using item = std::tuple<int, int, int>;

std::array<item, 5> constexpr weapons{ item
	{ 8, 4, 0},
	{10, 5, 0},
	{25, 6, 0},
	{40, 7, 0},
	{74, 8, 0} };
std::array<item, 6> constexpr armors{ item
	{  0, 0, 0}, // no armor
	{ 13, 0, 1},
	{ 31, 0, 2},
	{ 53, 0, 3},
	{ 75, 0, 4},
	{102, 0, 5} };
std::array<item, 4> constexpr rings1{ item
	{  0, 0, 0}, // no ring
	{ 25, 1, 0},
	{ 50, 2, 0},
	{100, 3, 0} };
std::array<item, 4> constexpr rings2{ item
	{ 0,  0, 0}, // no ring
	{20,  0, 1},
	{40,  0, 2},
	{80,  0, 3} };

int calc_damage(int damage, int armor) {
	return std::max(1, damage - armor);
}

bool do_battle(character player, character boss) {
	auto [p_hp, p_dmg, p_armor] = player;
	auto [b_hp, b_dmg, b_armor] = boss;

	int const p_attack = calc_damage(p_dmg, b_armor);
	int const b_attack = calc_damage(b_dmg, p_armor);

	while (true) {
		// player turn
		b_hp -= p_attack;
		if (b_hp <= 0)
			return true;

		// boss turn
		p_hp -= b_attack;
		if (p_hp <= 0)
			return false;
	}
}

int main() {
	int min_win_cost = std::numeric_limits<int>::max();
	int max_loose_cost = std::numeric_limits<int>::min();
	for (auto const& [w_cost,  w_dmg,  w_armor]  : weapons) {
	for (auto const& [a_cost,  a_dmg,  a_armor]  : armors) {
	for (auto const& [r1_cost, r1_dmg, r1_armor] : rings1) {
	for (auto const& [r2_cost, r2_dmg, r2_armor] : rings2) {
		int const cost  = w_cost  + a_cost  + r1_cost  + r2_cost;
		int const dmg   = w_dmg   + a_dmg   + r1_dmg   + r2_dmg;
		int const armor = w_armor + a_armor + r1_armor + r2_armor;

		character player{ 100, dmg, armor };
		character boss  { 104,   8,     1 };
		if (do_battle(player, boss)) {
			std::cout << "Victory! cost: " << cost << '\n';
			min_win_cost = std::min(cost, min_win_cost);
		}
		else {
			std::cout << "Defeat! cost: " << cost << '\n';
			max_loose_cost = std::max(cost, max_loose_cost);
		}
	}}}}

	std::cout << "Minimum cost to win:   " << min_win_cost << '\n';
	std::cout << "Maximum cost to loose: " << max_loose_cost << '\n';
}
