#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

#include <CGAL/Gmpz.h>

using namespace std;
using namespace CGAL;

Gmpz factorial(uint64_t n)
{
	static map<uint64_t, Gmpz> cache;

	uint64_t start = 1;
	Gmpz result = Gmpz(1);

	for (auto kv : cache) {
		if (kv.first <= n) {
			start = kv.first;
			result = kv.second;
		}
	}

	for (uint64_t i = start + 1; i <= n; i++) {
		result *= i;
	}

	cache.insert(pair<uint64_t, Gmpz>(n, result));

	return result;
}

Gmpz nCk(uint64_t n, uint64_t k)
{
	if (n < k)
		return 0;
	else
		return factorial(n) / k / factorial(n - k);
}

Gmpz even_pairs(vector<uint8_t>& numbers)
{
	Gmpz pairs;
	uint64_t *sums = new uint64_t[numbers.size()];

	sums[0] = numbers.at(0);

	for (uint64_t i = 1; i < numbers.size(); i++) {
		sums[i] = sums[i-1] + numbers.at(i);
	}

	uint64_t even = 0;
	uint64_t odd = 0;

	for (uint64_t i = 0; i < numbers.size(); i++) {
		if (sums[i] % 2 == 0)
			even++;
		else
			odd++;
	}

	pairs = even + nCk(even, 2) + nCk(odd, 2);

	return pairs;
}

void process_test(void)
{
	int64_t n;
	vector<uint8_t> numbers;

	cin >> n;

	if (n <= 0) {
		cout << "0" << endl;
		return;
	}

	for (int64_t i = 0; i < n; i++) {
		uint16_t next;

		cin >> next;
		numbers.push_back(next);
	}

	Gmpz pairs;

	pairs = even_pairs(numbers);
	cout << pairs << endl;
}

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	uint16_t t;

	cin >> t;

	for (uint16_t i = 0; i < t; i++)
		process_test();

	return EXIT_SUCCESS;
}
