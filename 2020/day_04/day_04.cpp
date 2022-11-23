#include <iostream>
#include <fstream>
#include <string_view>
#include <vector>
#include <algorithm>


int main() {
    // Read the input
    std::ifstream file("../day_04/input.txt");
    std::vector<char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    data.push_back('\n');
    data.push_back('\n');
    std::string_view input(data.data(), data.size());
    /*std::string_view input(R"(ecl:gry pid:860033327 eyr:2020 hcl:#fffffd
byr:1937 iyr:2017 cid:147 hgt:183cm

iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884
hcl:#cfa07d byr:1929

hcl:#ae17e1 iyr:2013
eyr:2024
ecl:brn pid:760753108 byr:1931
hgt:179cm

hcl:#cfa07d eyr:2025 pid:166559648
iyr:2011 ecl:brn hgt:59in

)");*/

    int num_valid_passports = 0;

    while (!input.empty()) {
        auto const passport_size = input.find("\n\n");

        std::string_view passport = input.substr(0, passport_size);
        auto const num_fields = std::count_if(passport.begin(), passport.end(), [](char c) { return c == ':'; });

        if (num_fields == 8) {
            num_valid_passports++;
        }
        else if (num_fields == 7) {
            auto const off = passport.find("cid:");
            num_valid_passports += (off == std::string_view::npos);
        }

        // Skip over the current passport
        input.remove_prefix(passport_size + 2);
    }

    std::cout << num_valid_passports;
}

// < 128
