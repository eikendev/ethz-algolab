#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

void process_test(void)
{
	int v1;
	long long v2;
	string v3;
	double v4;

	cin >> v1 >> v2 >> v3 >> v4;

	cout << std::fixed << std::setprecision(2);
	cout << v1 << " " << v2 << " " << v3 << " " << v4 << endl;
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
