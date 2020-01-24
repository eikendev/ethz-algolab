#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

void process_test(void)
{
	uint16_t n;
	cin >> n;

	vector<uint32_t> a(n);

	for (uint16_t i = 0; i < n; i++) {
		cin >> a[i];
		a[i]--;
	}

	vector<uint32_t> b(n);

	for (uint16_t i = 0; i < n; i++) {
		cin >> b[i];
		b[i]--;
	}

	vector<vector<uint32_t>> cost(n, vector<uint32_t>(n, 0));

	cost[0][0] = a[0] * b[0];

	for (uint16_t i = 1; i < n; i++) {
		cost[i][0] = cost[i - 1][0] + a[i] * b[0];
		cost[0][i] = cost[0][i - 1] + a[0] * b[i];
	}

	for (uint16_t i = 1; i < n; i++) {
		for (uint16_t j = 1; j < n; j++) {
			uint32_t a_extend = cost[i - 1][j];
			uint32_t b_extend = cost[i][j - 1];
			uint32_t no_extend = cost[i - 1][j - 1];
			cost[i][j] = a[i] * b[j] + min(no_extend, min(a_extend, b_extend));
		}
	}

	cout << cost[n - 1][n - 1] << endl;
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
