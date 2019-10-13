#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

typedef struct {
	uint32_t repair_parent; //< Parent is repaired, no need to repair this node.
	uint32_t repair_self; //< Parent is not repaired, and needs this node to be repaired.
	uint32_t repair_child; //< Parent is not repaired, and does not need this node to be repaired.
	uint16_t repair;
	bool valid;
} Costs;

void evaluate_costs(const vector<vector<uint32_t>>& children, vector<Costs>& costs, uint32_t node)
{
	uint32_t csum_rp = 0;
	uint32_t csum_rc = 0;
	int32_t maxrcdiff = numeric_limits<int32_t>::max();

	for (auto child : children[node]) {
		if (!costs[child].valid)
			evaluate_costs(children, costs, child);

		csum_rp += costs[child].repair_parent;
		csum_rc += costs[child].repair_child;

		maxrcdiff = min(maxrcdiff, (int32_t) costs[child].repair_self - (int32_t) costs[child].repair_child);
	}

	costs[node].repair_parent = min(costs[node].repair + csum_rp, csum_rc);
	costs[node].repair_self = (costs[node].repair + csum_rp);
	costs[node].repair_child = min(costs[node].repair + csum_rp, csum_rc + maxrcdiff);
	costs[node].valid = true;

	return;
}

void process_test(void)
{
	uint32_t n;

	cin >> n;

	vector<vector<uint32_t>> children(n);

	for (uint32_t k = 0; k < n - 1; k++) {
		uint32_t i, j;

		cin >> i >> j;
		children[i].push_back(j);
	}

	vector<Costs> costs(n);

	for (uint32_t k = 0; k < n; k++) {
		uint16_t c;

		cin >> c;
		costs[k].repair = c;
		costs[k].valid = false;
	}

	evaluate_costs(children, costs, 0);
	uint32_t cmin = min(costs[0].repair_self, costs[0].repair_child);

	cout << cmin << endl;
}

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	uint16_t t;

	cin >> t;

	for (uint16_t i = 0; i < t; i++) {
		process_test();
	}

	return EXIT_SUCCESS;
}
