#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

int64_t build_the_sum(vector<int16_t>& numbers)
{
	int64_t sum = 0;

	for (auto current : numbers) {
		sum += current;
	}

	return sum;
}

void process_test(void)
{
	int64_t n;
	vector<int16_t> numbers;

	cin >> n;

	if (n <= 0) {
		cout << "0" << endl;
		return;
	}

	for (int64_t i = 0; i < n; i++) {
		int16_t next;

		cin >> next;
		numbers.push_back(next);
	}

	int64_t sum;

	sum = build_the_sum(numbers);
	cout << sum << endl;
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
