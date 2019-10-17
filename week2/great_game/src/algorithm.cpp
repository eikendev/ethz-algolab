#include <cassert>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

typedef vector<vector<uint32_t>> Graph;

uint32_t winning_strategy(const Graph& g, uint32_t m, bool turn, vector<vector<int32_t>>& cache)
{
	if (m == g.size() - 1)
		return 0;

	if (cache[turn][m] > -1) {
		return cache[turn][m];
	}

	uint32_t distance;

	if (turn) {
		// It's our turn. Take the minimum.
		distance = numeric_limits<uint32_t>::max();

		for (auto v : g[m]) {
			uint32_t d = winning_strategy(g, v, !turn, cache);
			distance = min(distance, 1 + d);
		}
	} else {
		// It's their turn. Take the maximum.
		distance = numeric_limits<uint32_t>::min();

		for (auto v : g[m]) {
			uint32_t d = winning_strategy(g, v, !turn, cache);
			distance = max(distance, 1 + d);
		}
	}

	cache[turn][m] = distance;

	return distance;
}

void process_test(void)
{
	uint16_t n, m;
	cin >> n >> m;

	uint16_t r, b;
	cin >> r >> b;

	Graph g = vector<vector<uint32_t>>(n);

	for (uint16_t i = 0; i < m; i++) {
		uint16_t u, v;

		cin >> u >> v;
		g[u - 1].push_back(v - 1);
	}

	vector<vector<int32_t>> cache(2, vector<int32_t>(n, -1));

	uint32_t r_distance = winning_strategy(g, r - 1, true, cache);
	r_distance += (r_distance - (r_distance % 2));

	uint32_t b_distance = winning_strategy(g, b - 1, true, cache);
	b_distance += (b_distance - ((b_distance + 1) % 2));

	assert(r_distance != b_distance);

	bool holmes = r_distance <= b_distance;

	cout << !holmes << endl;
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
