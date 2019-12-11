#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

uint32_t maximize_money(vector<uint16_t>& values, uint16_t start, uint16_t end, vector<vector<int32_t>>& cache)
{
	switch (end - start) {
	case 0:
		return 0;
	case 1:
		return values[start];
	case 2:
		return max(values[start], values[end - 1]);
	}

	if (cache[start][end - 1] > 0)
		return cache[start][end - 1];

	uint32_t m1 = maximize_money(values, start + 2, end, cache);
	uint32_t m2 = maximize_money(values, start + 1, end - 1, cache);

	uint32_t money1 = values[start] + min(m1, m2);

	uint32_t m3 = maximize_money(values, start, end - 2, cache);
	uint32_t m4 = maximize_money(values, start + 1, end - 1, cache);

	uint32_t money2 = values[end - 1] + min(m3, m4);

	uint32_t money = max(money1, money2);

	cache[start][end - 1] = money;

	return money;
}

void process_test(void)
{
	uint16_t n;

	cin >> n;

	vector<uint16_t> values(n);

	for (auto& v : values) {
		cin >> v;
	}

	uint32_t money;

	vector<vector<int32_t>> cache(n, vector<int32_t>(n, -1));

	money = maximize_money(values, 0, values.size(), cache);
	cout << money << endl;
}

int main(void)
{
	ios_base::sync_with_stdio(false);

	uint16_t t;

	cin >> t;

	for (uint16_t i = 0; i < t; i++)
		process_test();

	return EXIT_SUCCESS;
}
