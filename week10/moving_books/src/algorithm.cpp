#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

void process_test(void)
{
	uint32_t n, m;
	cin >> n >> m;

	vector<uint32_t> strengths(n);
	uint32_t max_strength = 0;

	for (uint32_t i = 0; i < n; i++) {
		cin >> strengths[i];
		max_strength = max(max_strength, strengths[i]);
	}

	vector<uint32_t> weights(m);
	uint32_t max_weight = 0;

	for (uint32_t i = 0; i < m; i++) {
		cin >> weights[i];
		max_weight = max(max_weight, weights[i]);
	}

	if (max_weight > max_strength) {
		cout << "impossible" << endl;
		return;
	}

	vector<uint32_t> strength_counts(max_strength + 1, 0);

	for (const auto& s : strengths)
		strength_counts[s]++;

	vector<uint32_t> weight_counts(max_strength + 1, 0);

	for (const auto& w : weights)
		weight_counts[w]++;

	uint32_t transporters = 0;
	uint32_t transports = 0;
	uint32_t carry = 0;

	for (uint32_t i = max_strength; i > 0; i--) {
		carry += transports * strength_counts[i];

		const uint32_t from_carry = min(weight_counts[i], carry);
		weight_counts[i] -= from_carry;
		carry -= from_carry;

		transporters += strength_counts[i];
		const uint32_t new_transports = (uint32_t) ceil(weight_counts[i] / (double) transporters);
		transports += new_transports;

		if (weight_counts[i] % transporters > 0)
			carry += transporters - (weight_counts[i] % transporters);
	}

	cout << 3 * transports - 1 << endl;
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
