#include <cstdlib>
#include <iostream>
#include <array>
#include <set>
#include <stack>

using namespace std;

void print_array(int32_t *a, uint32_t n)
{
	for (uint32_t i = 0; i < n; i++) {
		cout << a[i] << " ";
	}

	cout << endl;
}

int32_t get_next_neighbor(set<uint32_t>& neighbors, int32_t *discovery)
{
	for (auto next : neighbors) {
		if (discovery[next] < 0)
			return next;
	}

	return -1;
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
	int32_t *discovery = new int32_t[n];
	int32_t *finishing = new int32_t[n];
	stack<uint32_t> process;

	for (uint16_t i = 0; i < n; i++) {
		neighbors[i] = set<uint32_t>();
		discovery[i] = -1;
		finishing[i] = -1;
	}

	for (uint32_t i = 0; i < m; i++) {
		uint32_t start, end;

		cin >> start >> end;
		neighbors[start].insert(end);
		neighbors[end].insert(start);
	}

	process.push(v);
	discovery[v] = 0;
	uint32_t step = 1;

	while (!process.empty()) {
		int32_t next;

		v = process.top();
		next = get_next_neighbor(neighbors[v], discovery);

		if (next < 0) {
			process.pop();
			finishing[v] = step;
		} else {
			process.push(next);
			discovery[next] = step;
		}

		step++;
	}

	print_array(discovery, n);
	print_array(finishing, n);
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
