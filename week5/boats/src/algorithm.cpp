#include <cstdlib>
#include <iostream>
#include <limits>
#include <queue>

using namespace std;

typedef struct {
	int32_t ring;
	int32_t length;
} Boat;

const auto BoatsCmp = [](const Boat& a, const Boat& b) { return a.ring > b.ring; };

typedef priority_queue<Boat, std::vector<Boat>, decltype(BoatsCmp)> Boats;

uint32_t max_boats(Boats& boats)
{
	uint32_t count = 0;
	int64_t cursor = numeric_limits<int32_t>::min();

	Boat best;
	int64_t best_cursor = cursor;
	bool found = false;

	while (!boats.empty()) {
		Boat b = boats.top();

		if (cursor > b.ring) {
			boats.pop();
			continue;
		}

		int64_t next_cursor = b.ring + max((int64_t) 0, b.length - (b.ring - cursor));

		if (!found || next_cursor < best_cursor) {
			found = true;
			best = b;
			best_cursor = next_cursor;
			boats.pop();
			continue;
		}

		if (best_cursor <= b.ring) {
			found = false;
			cursor = best_cursor;
			count++;
			continue;
		}

		boats.pop();
	}

	if (found)
		count++;

	return count;
}

void process_test(void)
{
	uint32_t n;

	cin >> n;

	Boats boats(BoatsCmp);

	for (uint32_t i = 0; i < n; i++) {
		Boat b;
		cin >> b.length >> b.ring;
		boats.push(b);
	}

	uint32_t count = max_boats(boats);
	cout << count << endl;
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
