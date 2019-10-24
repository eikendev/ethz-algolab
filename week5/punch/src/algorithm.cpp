#include <algorithm>
#include <bitset>
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

typedef bitset<100> Cart;

typedef struct {
	uint32_t cost;
	Cart cart;
} CacheEntry;

typedef vector<CacheEntry> Cache;

CacheEntry cheapest_punch(vector<Beverage>& beverages, Cache& cache, int32_t remaining)
{
	if (remaining <= 0)
		return {0, 0};

	if (cache[remaining].cost != 0)
		return cache[remaining];

	uint32_t best_cost = numeric_limits<uint32_t>::max();
	Cart best_cart;

	for (uint16_t i = 0; i < beverages.size(); i++) {
		const Beverage& b = beverages[i];

		CacheEntry ce = cheapest_punch(beverages, cache, remaining - b.volume);
		ce.cart[i] = 1;

		if (b.cost + ce.cost < best_cost)
			best_cart = ce.cart;
		else if (b.cost + ce.cost == best_cost)
			best_cart = (best_cart.count() >= ce.cart.count()) ? best_cart : ce.cart;

		best_cost = min(best_cost, b.cost + ce.cost);
	}

	cache[remaining].cost = best_cost;
	cache[remaining].cart = best_cart;

	return cache[remaining];
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

	CacheEntry ce;
	ce.cost = 0;
	Cache cache(k + 1, ce);

	cheapest_punch(beverages, cache, k);

	cout << cache[k].cost << " " << cache[k].cart.count() << endl;
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
