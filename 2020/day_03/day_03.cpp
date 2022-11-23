#include <iostream>
#include <fstream>
#include <array>
#include <numeric>

int test(std::array<char, 32> const& line, int column, int step) {
    return static_cast<int>(line[(column * step) % 31] == '#');
}

int main() {
    std::ifstream input("../day_03/input.txt");
    std::array<char, 32> line{0};
    input.getline(line.data(), line.size()); // skip first row

    int column = 1;
    std::array<int64_t, 5> num_trees{0};
    while (input.getline(line.data(), line.size())) {
        num_trees[0] += test(line, column, 1); // 1 1
        num_trees[1] += test(line, column, 3); // 3 1
        num_trees[2] += test(line, column, 5); // 5 1
        num_trees[3] += test(line, column, 7); // 7 1

        if ((column % 2) == 0)
            num_trees[4] += test(line, column/2, 1); // 1 2

        column++;
    }

    std::cout << std::accumulate(num_trees.begin(), num_trees.end(), 1ll, std::multiplies<int64_t>());
}
