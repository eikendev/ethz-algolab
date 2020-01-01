#include <cstdlib>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

using namespace std;

#define MAX(a, b) (a >= b ? a : b)

void process_test(void)
{
	uint32_t n;

	cin >> n;

	vector<int32_t> positions(n);
	int32_t max_index = numeric_limits<int32_t>::min();
	int32_t min_index = numeric_limits<int32_t>::max();

	for (uint32_t i = 0; i < n; i++) {
		cin >> positions[i];

		max_index = max(max_index, positions[i]);
		min_index = min(min_index, positions[i]);
	}

	uint32_t interval = abs(max_index - min_index) + 1;

	vector<uint32_t> accumulation(interval, 0);
	vector<uint32_t> isparasol(interval, 0);

	for (uint32_t i = 0; i < n; i++) {
		accumulation[positions[i] - min_index] = 1;
		isparasol[positions[i] - min_index] = 1;
	}

	for (uint32_t i = 1; i < interval; i++) {
		accumulation[i] += accumulation[i - 1];
	}

	vector<uint8_t> counts(interval, 0);
	vector<uint32_t> max_counts;
	uint8_t max_count = numeric_limits<uint8_t>::min();

	for (uint32_t i = 0; i < interval; i++) {
		uint32_t left = (i >= 101) ? accumulation[i - 101] : 0;
		uint32_t right = (i + 100 < interval) ? accumulation[i + 100] : accumulation[interval - 1];

		uint8_t count = right - left;
		counts[i] = count;

		if (count > max_count) {
			max_counts.clear();
		}

		max_count = MAX(max_count, count);

		if (count >= max_count) {
			max_counts.push_back(i);
		}
	}

	vector<uint8_t> distances(interval, numeric_limits<uint8_t>::min());

	for (uint32_t i = 0; i < max_counts.size(); i++) {
		uint32_t index = max_counts[i];

		for (int8_t j = 100; j >= 0; j--) {
			uint32_t test;

			test = index + j;

			if (test < interval) {
				if (isparasol[test] == 1) {
					distances[index] = j;
					break;
				}
			}

			test = index - j;

			if (test < interval) {
				if (isparasol[test] == 1) {
					distances[index] = j;
					break;
				}
			}
		}
	}

	uint8_t min_distance = numeric_limits<uint8_t>::max();
	vector<int32_t> winners;

	for (uint32_t i = 0; i < interval; i++) {
		if (counts[i] == max_count) {
			if (distances[i] < min_distance) {
				min_distance = distances[i];
				winners.clear();
			}

			if (distances[i] == min_distance) {
				int32_t p;

				p = i + min_index;
				winners.push_back(p);
			}
		}
	}

	cout << (uint16_t) max_count << " " << (uint16_t) min_distance << endl;

	string s = accumulate(winners.begin(), winners.end(), string(), [](const string& ss, const int32_t v) {
		return ss.empty() ? to_string(v) : ss + " " + to_string(v);
	});

	cout << s << endl;
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
