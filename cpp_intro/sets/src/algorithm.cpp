#include <cassert>
#include <cstdlib>
#include <iostream>
#include <set>

using namespace std;

void process_test(void)
{
	uint16_t q, a;
	int16_t b;
	set<int16_t> s;

	cin >> q;

	for (uint8_t i = 0; i < q; i++) {
		cin >> a >> b;

		switch (a) {
		case 0:
			s.insert(b);
			break;
		case 1:
			s.erase(b);
			break;
		default:
			assert(false);
			break;
		}
	}

	if (s.begin() == s.end()) {
		cout << "Empty" << endl;
		return;
	}

	for (auto v : s) {
		cout << v << " ";
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
