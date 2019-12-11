#include <cstdlib>
#include <iostream>

using namespace std;

void process_test(void)
{
	uint16_t n;
	int64_t result = 0;

	cin >> n;

	for (uint16_t i = 0; i < n; i++) {
		uint64_t next;

		cin >> next;
		result += next;
	}

	cout << result << endl;
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
