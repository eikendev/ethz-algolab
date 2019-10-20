#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector<vector<uint16_t>>> Network;

static uint16_t operations;
static bool success;

void min_operations(const vector<uint16_t>& target, const Network& switches, uint8_t flipped, uint8_t step, const vector<uint16_t>& minsums, const vector<uint16_t>& maxsums)
{
	vector<uint16_t> minsums1(minsums);
	vector<uint16_t> minsums2(minsums);
	vector<uint16_t> maxsums1(maxsums);
	vector<uint16_t> maxsums2(maxsums);

	bool minsums1_equal = true;
	bool minsums2_equal = true;
	bool maxsums1_equal = true;
	bool maxsums2_equal = true;

	bool minsums1_leq = true;
	bool minsums2_leq = true;
	bool maxsums1_geq = true;
	bool maxsums2_geq = true;

	for (uint8_t i = 0; i < minsums.size(); i++) {
		uint16_t minsum = min(switches[step][i][0], switches[step][i][1]);
		uint16_t maxsum = max(switches[step][i][0], switches[step][i][1]);

		minsums1[i] += switches[step][i][0] - minsum;
		minsums2[i] += switches[step][i][1] - minsum;
		maxsums1[i] += switches[step][i][0] - maxsum;
		maxsums2[i] += switches[step][i][1] - maxsum;

		minsums1_equal = minsums1_equal and (minsums1[i] == target[i]);
		minsums2_equal = minsums2_equal and (minsums2[i] == target[i]);
		maxsums1_equal = maxsums1_equal and (maxsums1[i] == target[i]);
		maxsums2_equal = maxsums2_equal and (maxsums2[i] == target[i]);

		minsums1_leq = minsums1_leq and (minsums1[i] <= target[i]);
		minsums2_leq = minsums2_leq and (minsums2[i] <= target[i]);
		maxsums1_geq = maxsums1_geq and (maxsums1[i] >= target[i]);
		maxsums2_geq = maxsums2_geq and (maxsums2[i] >= target[i]);

		if ((!minsums1_leq || !maxsums1_geq) && (!minsums2_leq || !maxsums2_geq))
			break;
	}

	if (step == switches.size() - 1) {
		if (minsums1_equal) {
			operations = flipped;
			success = true;
		} else if (minsums2_equal) {
			operations = flipped + 1;
			success = true;
		}

		return;
	}

	if (minsums1_leq && maxsums1_geq)
		min_operations(target, switches, flipped, step + 1, minsums1, maxsums1);

	if (flipped + 1 < operations && minsums2_leq && maxsums2_geq)
		min_operations(target, switches, flipped + 1, step + 1, minsums2, maxsums2);
}

void process_test(void)
{
	uint16_t n, m;

	cin >> n >> m;

	vector<uint16_t> target(m);

	for (uint16_t i = 0; i < m; i++)
		cin >> target[i];

	Network switches(n, vector<vector<uint16_t>>(m, vector<uint16_t>(2)));
	vector<uint16_t> minsums(target.size(), 0);
	vector<uint16_t> maxsums(target.size(), 0);

	for (uint16_t i = 0; i < n; i++) {
		for (uint16_t j = 0; j < m; j++) {
			cin >> switches[i][j][0];
			cin >> switches[i][j][1];
			minsums[j] += min(switches[i][j][0], switches[i][j][1]);
			maxsums[j] += max(switches[i][j][0], switches[i][j][1]);
		}
	}

	operations = switches.size() + 1;
	success = false;
	min_operations(target, switches, 0, 0, minsums, maxsums);

	if (success)
		cout << operations << endl;
	else
		cout << "impossible" << endl;
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
