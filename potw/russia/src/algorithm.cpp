#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector<vector<int32_t>>> Cache;

uint16_t n, m, k;

uint32_t largest_winnings(const vector<uint16_t>& values, uint32_t turn, uint32_t left, uint32_t right, Cache& cache)
{
	if (left == right) {
		if (turn == k)
			return values[left];
		else
			return 0;
	}

	if (cache[turn == k][left][right] != -1)
		return cache[turn == k][left][right];

	uint32_t left_add = (turn == k) ? values[left] : 0;
	uint32_t left_sum = left_add + largest_winnings(values, (turn + 1) % m, left + 1, right, cache);

	uint32_t right_add = (turn == k) ? values[right] : 0;
	uint32_t right_sum = right_add + largest_winnings(values, (turn + 1) % m, left, right - 1, cache);

	uint32_t max_sum = max(left_sum, right_sum);
	uint32_t min_sum = min(left_sum, right_sum);

	cache[0][left][right] = min_sum;
	cache[1][left][right] = max_sum;

	uint32_t sum;

	if (turn == k)
		sum = max_sum;
	else
		sum = min_sum;

	return sum;
}

void process_test(void)
{
	cin >> n >> m >> k;

	vector<uint16_t> values(n);

	for (uint16_t i = 0; i < n; i++) {
		cin >> values[i];
	}

	Cache cache = vector<vector<vector<int32_t>>>(2, vector<vector<int32_t>>(n, vector<int32_t>(n, -1)));

	uint32_t sum;

	sum = largest_winnings(values, 0, 0, n - 1, cache);
	cout << sum << endl;
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
