#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

typedef array<uint16_t, 2> Room;
typedef vector<Room> Switch;
typedef vector<Switch> Network;

static uint16_t operations;
static bool success;

static const auto SumSwitch = [](uint16_t acc, const Room& r) {
	return acc + r[0] + r[1];
};

void min_operations(const vector<uint16_t>& target, const Network& switches, uint8_t flipped, uint8_t step, const vector<uint16_t>& minsums, const vector<uint16_t>& maxsums)
{
	vector<uint16_t> minsums1(minsums);
	vector<uint16_t> minsums2(minsums);
	vector<uint16_t> maxsums1(maxsums);
	vector<uint16_t> maxsums2(maxsums);

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

		minsums1_leq = minsums1_leq and (minsums1[i] <= target[i]);
		minsums2_leq = minsums2_leq and (minsums2[i] <= target[i]);
		maxsums1_geq = maxsums1_geq and (maxsums1[i] >= target[i]);
		maxsums2_geq = maxsums2_geq and (maxsums2[i] >= target[i]);

		if ((!minsums1_leq || !maxsums1_geq) && (!minsums2_leq || !maxsums2_geq))
			break;
	}

	if (step == switches.size() - 1) {
		if (minsums1_leq && maxsums1_geq) {
			operations = flipped;
			success = true;
		} else if (minsums2_leq && maxsums2_geq) {
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

	Network switches(n, vector<Room>(m));

	for (uint16_t i = 0; i < n; i++) {
		for (uint16_t j = 0; j < m; j++) {
			cin >> switches[i][j][0];
			cin >> switches[i][j][1];
		}
	}

	// Sort the entries by decreasing influence.
	// Influence is the sum of all lights a switch controls.
	sort(switches.begin(), switches.end(), [](const Switch& a, const Switch& b) {
		uint16_t sum_a = accumulate(a.begin(), a.end(), (uint16_t) 0, SumSwitch);
		uint16_t sum_b = accumulate(b.begin(), b.end(), (uint16_t) 0, SumSwitch);
		return sum_a > sum_b;
	});

	vector<uint16_t> minsums(target.size(), 0);
	vector<uint16_t> maxsums(target.size(), 0);

	for (uint16_t i = 0; i < n; i++) {
		for (uint16_t j = 0; j < m; j++) {
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

int main(void)
{
	ios_base::sync_with_stdio(false);

	uint16_t t;

	cin >> t;

	for (uint16_t i = 0; i < t; i++)
		process_test();

	return EXIT_SUCCESS;
}
