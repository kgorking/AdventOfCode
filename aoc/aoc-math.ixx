export module aoc:math;
import std;

export namespace kg {
// Calculate the binomial coefficient.
// n and k must be greater than 0, k must be less than n
std::uint64_t binomial_coefficient(int n, int k) {
	double res = 1;
	for (int i = 1; i <= k; ++i)
		res = res * (n - k + i) / i;
	return (std::uint64_t)(res + 0.01);

	/*if (n <= 0 || k < 0 || k > n) [[unlikely]]
		std::terminate();
	if (k == 0 or k == n)
		return 1;
	if (k > n - k) // take advantage of symmetry
		k = n - k;
	return std::lround(std::exp(std::lgamma(n + 1) - std::lgamma(k + 1) - std::lgamma(n - k + 1)));*/
}
} // namespace kg
