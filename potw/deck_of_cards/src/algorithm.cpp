#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

void process_test(void)
{
	uint32_t n, k;

	cin >> n >> k;

	uint32_t last = 0;
	vector<int32_t> sums(n);

	for (uint32_t i = 0; i < n; i++) {
		uint32_t next;

		cin >> next;

		sums[i] = last + next;
		last += next;
	}

	uint32_t start = 0;
	uint32_t end = 0;
	uint32_t best_start = start;
	uint32_t best_end = end;
	int32_t best_value = INT32_MAX;
	bool has_options = true;

	do {
		int32_t current;

		int32_t subtract = (start > 0) ? sums[start - 1] : 0;
		current = k - (sums[end] - subtract);

		if (abs(current) < best_value) {
			best_start = start;
			best_end = end;
			best_value = abs(current);

			if (current == 0)
				goto end;
		}

		if (current < 0) {
			if (start + 1 <= end)
				start++;
			else {
				if (end + 1 == n)
					has_options = false;
				else
					end++;
			}

		} else {
			if (end + 1 == n)
				has_options = false;

			end++;
		}
	} while (has_options);

end:
	cout << best_start << " " << best_end << endl;
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
