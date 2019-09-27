#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

void process_test(void)
{
	uint16_t n;

	cin >> n;

	vector<uint32_t> occurences(n);

	for (uint32_t i = 0; i < n; i++) {
		uint32_t m;

		cin >> m;
		occurences[i] = m;
	}

	vector<pair<uint32_t, uint16_t>> sentence;

	for (uint32_t i = 0; i < n; i++) {
		for (uint32_t j = 0; j < occurences[i]; j++) {
			uint32_t p;

			cin >> p;
			sentence.push_back(pair<uint32_t, uint16_t>(p, i));
		}
	}

	struct {
		bool operator() (const pair<uint32_t, uint16_t>& a, const pair<uint32_t, uint16_t>& b) const {
			return a.first < b.first;
		}
	} pair_compare;

	sort(sentence.begin(), sentence.end(), pair_compare);

	uint32_t start = 0;
	uint32_t end = 0;
	vector<uint32_t> count(n, 0);

	{
		uint16_t considered = 0;

		// Calculate the initial range.
		// Count how often a word is found in that range.
		while (considered != n) {
			uint16_t word = sentence[end].second;
			count[word]++;

			if (count[word] == 1)
				considered++;

			if (considered < n)
				end++;
		}
	}

	uint32_t best_start = start;
	uint32_t best_end = end;
	uint32_t best_length = sentence[end].first - sentence[start].first + 1;

	while (best_length > n) {
		int32_t target = -1;

		// Reduce the tail.
		while (start <= end) {
			uint32_t word = sentence[start].second;
			count[word]--;
			start++;

			if (count[word] == 0) {
				target = word;
				break;
			}

			uint32_t length = sentence[end].first - sentence[start].first + 1;
			if (length < best_length) {
				best_start = start;
				best_end = end;
				best_length = length;
			}
		}

		// Increase the head.
		while (end < sentence.size()) {
			end++;
			uint32_t word = sentence[end].second;
			count[word]++;

			if (target == -1)
				break;

			if (count[word] == 1)
				break;
		}

		if (end == sentence.size())
			break;

		uint32_t length = sentence[end].first - sentence[start].first + 1;
		if (length < best_length) {
			best_start = start;
			best_end = end;
			best_length = length;
		}
	}

	cout << best_length << endl;
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
