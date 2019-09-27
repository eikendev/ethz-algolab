#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void process_test(void)
{
	uint32_t n;
	vector<int16_t> numbers;

	cin >> n;

	for (uint32_t i = 0; i < n; i++) {
		int16_t next;

		cin >> next;
		numbers.push_back(next);
	}

	uint16_t x;

	cin >> x;

	switch (x) {
	case 0:
		sort(numbers.begin(), numbers.end(), less<int32_t>());
		break;
	case 1:
		sort(numbers.begin(), numbers.end(), greater<int32_t>());
		break;
	default:
		assert(false);
		break;
	}

	for (auto v : numbers) {
		cout << v << " ";
	}

	cout << endl;
}

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	uint16_t t;

	cin >> t;

	for (uint8_t i = 0; i < t; i++) {
		process_test();
	}

	return EXIT_SUCCESS;
}
