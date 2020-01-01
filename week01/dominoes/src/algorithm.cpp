#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

#include <CGAL/Gmpz.h>

#define MAX(a, b) ((a > b) ? a : b)

using namespace std;
using namespace CGAL;

Gmpz dominoes(vector<uint32_t>& heights)
{
	Gmpz falling = 1;

	uint32_t left = heights.at(0);

	for (uint64_t i = 1; i < heights.size(); i++) {
		if (left <= 1)
			break;

		falling++;
		left = MAX(left - 1, heights.at(i));
	}

	return falling;
}

void process_test(void)
{
	int64_t n;
	vector<uint32_t> heights;

	cin >> n;

	if (n <= 0) {
		cout << "0" << endl;
		return;
	}

	for (int64_t i = 0; i < n; i++) {
		uint32_t next;

		cin >> next;
		heights.push_back(next);
	}

	Gmpz falling;

	falling = dominoes(heights);
	cout << falling << endl;
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
