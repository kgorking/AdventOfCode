#include <iostream>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <array>

int main() {
	std::string const molecule = "CRnSiRnCaPTiMgYCaPTiRnFArSiThFArCaSiThSiThPBCaCaSiRnSiRnTiTiMgArPBCaPMgYPTiRnFArFArCaSiRnBPMgArPRnCaPTiRnFArCaSiThCaCaFArPBCaCaPTiTiRnFArCaSiRnSiAlYSiThRnFArArCaSiRnBFArCaCaSiRnSiThCaCaCaFYCaPTiBCaSiThCaSiThPMgArSiRnCaPBFYCaCaFArCaCaCaCaSiThCaSiRnPRnFArPBSiThPRnFArSiRnMgArCaFYFArCaSiRnSiAlArTiTiTiTiTiTiTiRnPMgArPTiTiTiBSiRnSiAlArTiTiRnPMgArCaFYBPBPTiRnSiRnMgArSiThCaFArCaSiThFArPRnFArCaSiRnTiBSiThSiRnSiAlYCaFArPRnFArSiThCaFArCaCaSiThCaCaCaSiRnPRnCaFArFYPMgArCaPBCaPBSiRnFYPBCaFArCaSiAl";

	std::array<std::pair<std::string, std::string>, 43> replacements{ {
		{"Al", "ThF"},
		{"Al", "ThRnFAr"},
		{"B", "BCa"},
		{"B", "TiB"},
		{"B", "TiRnFAr"},
		{"Ca", "CaCa"},
		{"Ca", "PB"},
		{"Ca", "PRnFAr"},
		{"Ca", "SiRnFYFAr"},
		{"Ca", "SiRnMgAr"},
		{"Ca", "SiTh"},
		{"F", "CaF"},
		{"F", "PMg"},
		{"F", "SiAl"},
		{"H", "CRnAlAr"},
		{"H", "CRnFYFYFAr"},
		{"H", "CRnFYMgAr"},
		{"H", "CRnMgYFAr"},
		{"H", "HCa"},
		{"H", "NRnFYFAr"},
		{"H", "NRnMgAr"},
		{"H", "NTh"},
		{"H", "OB"},
		{"H", "ORnFAr"},
		{"Mg", "BF"},
		{"Mg", "TiMg"},
		{"N", "CRnFAr"},
		{"N", "HSi"},
		{"O", "CRnFYFAr"},
		{"O", "CRnMgAr"},
		{"O", "HP"},
		{"O", "NRnFAr"},
		{"O", "OTi"},
		{"P", "CaP"},
		{"P", "PTi"},
		{"P", "SiRnFAr"},
		{"Si", "CaSi"},
		{"Th", "ThCa"},
		{"Ti", "BP"},
		{"Ti", "TiTi"},
		{"e", "HF"},
		{"e", "NAl"},
		{"e", "OMg"},
	} };

	std::string temp;
	std::unordered_set<size_t> hashes;
	auto const hasher = std::hash<std::string>();
	for (auto const& [from, to] : replacements) {
		auto it = std::search(molecule.begin(), molecule.end(), from.begin(), from.end());
		while (it != molecule.end()) {
			auto const pos = std::distance(molecule.begin(), it);
			temp = molecule;
			temp.replace(pos, from.length(), to);

			hashes.insert(hasher(temp));

			it = std::search(it + from.length(), molecule.end(), from.begin(), from.end());
		}
	}
	std::cout << "Number of distinct molecules that can be created: " << hashes.size() << '\n';


	// Sort the replacements so the largest molecules appear first. Leave electrons at end
	std::sort(replacements.begin(), replacements.end() - 3, [](auto const& l, auto const& r) {
		return l.second.length() > r.second.length();
	});

	int steps = 0;
	temp = molecule;
	for (auto i = replacements.begin(); i != replacements.end();) {
		auto const& [from, to] = *i;

		auto it = std::search(temp.begin(), temp.end(), to.begin(), to.end());
		if (it != temp.end()) {
			auto const pos = std::distance(temp.begin(), it);

			temp.replace(pos, to.length(), from);
			steps++;
			std::cout << temp << "\n\n";

			// Restart the search
			i = replacements.begin();
		}
		else
			++i;
	}
	std::cout << "Steps needed to make molecule: " << steps << '\n';
}
