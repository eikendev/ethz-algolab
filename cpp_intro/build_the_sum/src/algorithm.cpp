#include <cstdlib>
#include <iostream>

using namespace std;

void process_addition(void)
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

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	uint16_t t;

	cin >> t;

	for (uint16_t i = 0; i < t; i++) {
		process_addition();
	}

	return EXIT_SUCCESS;
}
