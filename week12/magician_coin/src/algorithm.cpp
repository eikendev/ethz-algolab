#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector<pair<bool, double>>> Cache;

double calculate_sum(const vector<double>& p, const uint16_t day, const uint32_t capital, const uint16_t goal, Cache& cache)
{
	if (capital >= goal)
		return 1;
	else if (capital == 0)
		return 0;
	else if (day == p.size())
		return 0;

	if (cache[day][capital].first)
		return cache[day][capital].second;

	double maximum = 0;

	for (uint32_t c = 0; c <= min(capital, goal - capital); c++) {
		double win = p[day] * calculate_sum(p, day + 1, capital + c, goal, cache);
		double lose = (1 - p[day]) * calculate_sum(p, day + 1, capital - c, goal, cache);

		maximum = max(maximum, win + lose);
	}

	cache[day][capital] = make_pair(true, maximum);

	return maximum;
}

void process_test(void)
{
	uint16_t n, m;
	uint32_t k;
	cin >> n >> k >> m;

	vector<double> p(n);

	for (uint16_t i = 0; i < n; i++)
		cin >> p[i];

	Cache cache(n, vector<pair<bool, double>>(m, make_pair(false, 0)));
	double max_sum = calculate_sum(p, 0, k, m, cache);

	cout << fixed << setprecision(5);
	cout << max_sum << endl;
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
