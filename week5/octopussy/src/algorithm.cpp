#include <cstdlib>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

typedef struct {
	uint16_t index;
	uint32_t detonation;
} Bomb;

const auto BombCmp = [](const Bomb& a, const Bomb& b) { return a.detonation > b.detonation; };
typedef priority_queue<Bomb, std::vector<Bomb>, decltype(BombCmp)> Bombs;

uint16_t disable(vector<bool>& active, uint16_t i)
{
	active[i] = false;

	if (i >= active.size() / 2)
		return 1;

	uint16_t count = 1;
	count += active[2 * i + 1] ? disable(active, 2 * i + 1) : 0;
	count += active[2 * i + 2] ? disable(active, 2 * i + 2) : 0;

	return count;
}

bool is_deactivatable(Bombs& bombs)
{
	uint16_t n = bombs.size();
	vector<bool> active(n, true);
	uint32_t step = 0;

	while (!bombs.empty()) {
		Bomb b = bombs.top();
		bombs.pop();

		if (!active[b.index])
			continue;

		uint16_t count = disable(active, b.index);
		step += count;

		if (step > b.detonation)
			return false;
	}

	return true;
}

void process_test(void)
{
	uint16_t n;

	cin >> n;

	Bombs bombs(BombCmp);

	for (uint16_t i = 0; i < n; i++) {
		Bomb b;
		b.index = i;
		cin >> b.detonation;
		bombs.push(b);
	}

	bool result = is_deactivatable(bombs);

	if (result)
		cout << "yes" << endl;
	else
		cout << "no" << endl;
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
