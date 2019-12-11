#include <cstdlib>
#include <iostream>
#include <array>
#include <set>
#include <queue>

using namespace std;

void print_array(int32_t *a, uint32_t n)
{
	for (uint32_t i = 0; i < n; i++) {
		cout << a[i] << " ";
	}

	cout << endl;
}

void process_test(void)
{
	uint32_t n, m, v;

	cin >> n >> m >> v;

	if (n == 0) {
		cout << endl << endl;
		return;
	}

	set<uint32_t> *neighbors = new set<uint32_t>[n];
	int32_t *distance = new int32_t[n];
	queue<uint32_t> process;

	for (uint16_t i = 0; i < n; i++) {
		neighbors[i] = set<uint32_t>();
		distance[i] = -1;
	}

	for (uint32_t i = 0; i < m; i++) {
		uint32_t start, end;

		cin >> start >> end;
		neighbors[start].insert(end);
		neighbors[end].insert(start);
	}

	process.push(v);
	distance[v] = 0;

	while (!process.empty()) {
		v = process.front();
		process.pop();

		for (auto neighbor : neighbors[v]) {
			if (distance[neighbor] < 0) {
				process.push(neighbor);
				distance[neighbor] = distance[v] + 1;
			}
		}
	}

	print_array(distance, n);
}

int main(void)
{
	ios_base::sync_with_stdio(false);

	uint16_t t;

	cin >> t;

	for (uint8_t i = 0; i < t; i++)
		process_test();

	return EXIT_SUCCESS;
}
