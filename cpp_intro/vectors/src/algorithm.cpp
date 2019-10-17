#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

void process_test(void)
{
	uint16_t n;
	vector<int16_t> v;
	int16_t next;

	cin >> n;

	for (uint8_t i = 0; i < n; i++) {
		cin >> next;
		v.push_back(next);
	}

	int16_t pos, first, last;

	cin >> pos >> first >> last;

	v.erase(v.begin() + pos);
	v.erase(v.begin() + first, v.begin() + last + 1);

	for (auto out : v) {
		cout << out << " ";
	}

	cout << endl;
}

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	uint16_t t;

	cin >> t;

	for (uint8_t i = 0; i < t; i++)
		process_test();

	return EXIT_SUCCESS;
}
