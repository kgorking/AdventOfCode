export module aoc:string;

import std;
import :numeric;

export namespace kg {
std::vector<int> make_z_array(std::string_view pattern, std::string_view text) {
	auto const blob = std::to_array({ pattern, text });
	auto s = blob | std::views::join_with('#') | std::ranges::to<std::string>();

	int const n = blob.size();
	std::vector<int> z(n);

	int x = 0, y = 0;

	for (int i = 1; i < n; i++) {
		z[i] = std::max(0, std::min(z[i - x], y - i + 1));
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
			x = i;
			y = i + z[i];
			z[i]++;
		}
	}

	return z;
}

struct anagram_pattern_matcher {
	anagram_pattern_matcher(std::string_view pattern) {
		pattern_size = pattern.size();
		count_pattern = kg::histogram_vec(pattern);
	}

	int matches(std::string_view text) const {
		auto count_text = kg::histogram_vec(text.substr(0, pattern_size));

		int found = 0;
		for (int i = pattern_size; i < text.size(); i++) {
			if (compare_hist(count_text))
				found += 1;

			// Add current character to current window
			// Remove the first character of previous window
			count_text[text[i]] += 1;
			count_text[text[i - pattern_size]] -= 1;
		}

		return found + compare_hist(count_text);
	}

private:
	bool compare_hist(std::vector<char> const& count_text) const {
		int const wildcards = count_text['?'];

		for (int i = 0; i < count_pattern.size(); i++) {
			if (i == '?')
				continue;

			if (count_pattern[i] != count_text[i] && count_pattern[i] - wildcards != count_text[i])
				return false;
		}

		return true;
	}

private:
	std::vector<char> count_pattern;
	int pattern_size;
};

struct wildcard_pattern_matcher {
	wildcard_pattern_matcher(std::string_view pattern) : pattern (pattern) {
	}

	int matches(std::string_view const text) const {
		int i = 0, j = 0, startIndex = -1, match = 0;

		while (i < text.length()) {
			if (j < pattern.length() && (pattern[j] == '?' || pattern[j] == text[i])) {
				// Characters match or '?' in pattern matches any character.
				i++;
				j++;
			} else if (j < pattern.length() && pattern[j] == '*') {
				// Wildcard character '*', mark the current position in the pattern and the text as a proper match.
				startIndex = j;
				match = i;
				j++;
			} else if (startIndex != -1) {
				// No match, but a previous wildcard was found. Backtrack to the last '*' character position and try for a different match.
				j = startIndex + 1;
				match++;
				i = match;
			} else {
				// If none of the above cases comply, the pattern does not match.
				return false;
			}
		}

		// Consume any remaining '*' characters in the given pattern.
		while (j < pattern.length() && pattern[j] == '*') {
			j++;
		}

		// If we have reached the end of both the pattern and the text, the pattern matches the text.
		return j == pattern.length();
	}

private:
	std::string_view const pattern;
};
} // namespace kg
