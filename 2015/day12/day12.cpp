#include <iostream>
#include <vector>
#include <map>
#include <any>
#include <charconv>

#include "input.h"

using string = std::string_view;
using value = std::any;
using array = std::vector<value>;
using object = std::map<string, value>;

value parse(std::string_view& input);

string parse_string(std::string_view& s) {
	int index = 1;
	while (s[index] != '"')
		index++;
	auto val = s.substr(1, index - 1);
	s = s.substr(index + 1); // +1 to skip "
	return val;
}

int parse_number(std::string_view& s) {
	int index = 1;
	while (isdigit(s[index]))
		index++;

	int value = 0;
	std::from_chars(s.data(), s.data() + index, value);
	s = s.substr(index);
	return value;
}

array parse_array(std::string_view& s) {
	array arr;
	s = s.substr(1); // skip [

	while (s[0] != ']') {
		arr.push_back(parse(s));

		if (s[0] == ',') {
			s = s.substr(1);
		}
	}
	s = s.substr(1); // skip ]

	return arr;
}

object parse_object(std::string_view& s) {
	object o;
	s = s.substr(1); // skip {

	while (s[0] != '}') {
		string key = parse_string(s);

		if (s[0] == ':')
			s = s.substr(1);
		else
			throw;

		auto val = parse(s);
		o[key] = val;

		if (s[0] == ',')
			s = s.substr(1);
	}
	s = s.substr(1); // skip }

	return o;
}

value parse(std::string_view & input) {
	if (input[0] == '[')
		return parse_array(input);
	if (input[0] == '{')
		return parse_object(input);
	if (input[0] == '"')
		return parse_string(input);
	if (isdigit(input[0]) || input[0] == '-')
		return parse_number(input);
	return {};
}

int sum(std::any value) {
	// array
	if (typeid(array) == value.type()) {
		array arr{ std::any_cast<array>(value) };
		int arr_sum = 0;
		for (auto const& val : arr)
			arr_sum += sum(val);
		return arr_sum;
	}

	// int
	else if (typeid(int) == value.type()) {
		return std::any_cast<int>(value);
	}

	// object
	else if (typeid(object) == value.type()) {
		object o{ std::any_cast<object>(value) };

		int obj_sum = 0;
		for (auto const& [k, v] : o) {

			// Skip object with 'red' key/value
			if (k == "red")
				return 0;
			if (v.type() == typeid(string) && std::any_cast<string>(v) == "red")
				return 0;

			obj_sum += sum(v);
		}
		return obj_sum;
	}

	// everything else
	else
		return 0;
}

int main() {
	// test-input
	//std::string_view input = R"([1,2,3])";
	//std::string_view input = R"([1,{"c":"red","b":2},3])";
	//std::string_view input = R"({"d":"red","e":[1,2,3,4],"f":5})";
	//std::string_view input = R"([1,"red",5])";
	std::string_view input = ::input;

	auto const result = parse(input);
	std::cout << "sum of all numbers: " << sum(result) << '\n';
}
