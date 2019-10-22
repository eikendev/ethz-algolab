#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

typedef struct {
	uint16_t cost;
	uint16_t volume;
} Beverage;

static const auto BeverageCmp = [](const Beverage& a, const Beverage& b) {
	return (double) a.cost / a.volume < (double) b.cost / b.volume;
};

typedef array<uint16_t, 100> Configuration;

static uint32_t best_cost;
static uint16_t best_count;

void cheapest_punch(vector<Beverage>& beverages, Configuration cart, uint16_t target, uint32_t volume, uint32_t cost, uint16_t count, uint16_t offset)
{
	if (volume >= target) {
		if (cost < best_cost)
			best_count = count;

		best_count = max(best_count, count);
		best_cost = min(best_cost, cost);
		return;
	}

	for (uint16_t i = offset; i < beverages.size(); i++) {
		const Beverage& b = beverages[i];

		int32_t padding;
		padding = target - volume - b.volume;
		padding = max(padding, (int32_t) 0);
		padding = ceil(padding / (double) beverages[0].volume);
		padding = padding * beverages[0].cost;

 	 	if (cost + b.cost + padding > best_cost)
 	 		continue;

		uint16_t d = (cart[i] == 0) ? 1 : 0;
		cart[i]++;
		cheapest_punch(beverages, cart, target, volume + b.volume, cost + b.cost, count + d, i);
		cart[i]--;
	}
}

void process_test(void)
{
	uint16_t n, k;

	cin >> n >> k;

	vector<Beverage> beverages(n);

	for (uint16_t i = 0; i < n; i++) {
		Beverage b;
		cin >> b.cost;
		cin >> b.volume;
		beverages[i] = b;
	}

	sort(beverages.begin(), beverages.end(), BeverageCmp);

	Configuration cart;
	cart.fill(0);
	best_cost = numeric_limits<uint32_t>::max();
	best_count = numeric_limits<uint16_t>::max();

	cheapest_punch(beverages, cart, k, 0, 0, 0, 0);

	cout << best_cost << " " << best_count << endl;
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
