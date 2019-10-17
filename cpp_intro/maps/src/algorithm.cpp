#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>

using namespace std;

void process_test(void)
{
	uint16_t q;
	map<string, multiset<uint16_t>> m;

	cin >> q;

	for (uint8_t i = 0; i < q; i++) {
		uint16_t a;
		string b;

		cin >> a >> b;

		if (a == 0) {
			m.erase(b);
		} else {
			auto value = m.find(b);

			if (value == m.end()) {
				multiset<uint16_t> ms;
				ms.insert(a);
				m.insert(pair<string, multiset<uint16_t>>(b, ms));
			} else {
				value->second.insert(a);
			}
		}
	}

	string key;
	cin >> key;

	auto value = m.find(key);

	if (value == m.end() or value->second.size() == 0) {
		cout << "Empty" << endl;
		return;
	}

	for (auto v : value->second) {
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
