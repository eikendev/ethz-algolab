#include <cstdlib>
#include <iostream>
#include <tuple>
#include <vector>
#include <cassert>

using namespace std;

typedef struct {
	bool exists;
	uint32_t from;
	uint32_t to;
	uint32_t length;
} Interval;

typedef vector<vector<tuple<bool, bool, uint32_t>>> Cache;

pair<bool, uint32_t> evaluate(const vector<Interval>& intervals, const uint32_t n, const uint32_t m, Cache& cache)
{
	if (m == 0)
		return make_pair(true, 0);
	if (n >= intervals.size())
		return make_pair(false, 0);
	if (get<0>(cache[n][m]))
		return make_pair(get<1>(cache[n][m]), get<2>(cache[n][m]));

	bool success; uint32_t result;
	tie(success, result) = evaluate(intervals, n + 1, m, cache);

	if (intervals[n].exists) {
		bool success2; uint32_t result2;
		tie(success2, result2) = evaluate(intervals, intervals[n].to + 1,  m - 1, cache);
		result2 += intervals[n].length;
		success = success or success2;

		if (success2)
			result = max(result, result2);
	}

	cache[n][m] = make_tuple(true, success, result);

	return make_pair(success, result);
}

void process_test(void)
{
	uint32_t n, k;
	uint16_t m;

	cin >> n >> m >> k;

	vector<uint16_t> values(n);

	for (uint32_t i = 0; i < n; i++)
		cin >> values[i];

	vector<Interval> intervals(n, {false, 0, 0, 0});
	uint32_t istart = 0;
	uint32_t iend = 0;
	uint32_t isum = values[0];

	while (iend < n) {
		if (isum == k) {
			Interval interval = {true, istart, iend, iend - istart + 1};
			intervals[istart] = interval;
		}

		if (isum < k or istart == iend) {
			iend++;
			isum += (iend < n) ? values[iend] : 0;
		} else {
			isum -= values[istart];
			istart++;
		}
	}

	if (intervals.size() < m) {
		cout << "fail" << endl;
		return;
	}

	Cache cache(n, vector<tuple<bool, bool, uint32_t>>(m + 1, make_tuple(false, false, 0)));

	bool success; uint32_t result;
	tie(success, result) = evaluate(intervals, 0, m, cache);

	if (success)
		cout << result << endl;
	else
		cout << "fail" << endl;
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
