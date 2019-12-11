#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

typedef struct {
	uint32_t l[20];
} Lengths;

struct LengthsCmp {
    bool operator()(const vector<uint32_t>& a, const vector<uint32_t>& b) const {
    	uint8_t n = a.size();

    	for (uint8_t i = 0; i < n - 1; i++) {
    		if (a[i] < b[i])
        		return true;
    		if (a[i] > b[i])
        		return false;
		}

		return a[n - 1] < b[n - 1];
    }
};

static inline bool subset_is_length(const Lengths& l, uint8_t n, uint32_t subset, uint64_t target)
{
	uint64_t length = 0;

	for (uint8_t i = 0; i < n && subset > 0; i++) {
		if ((subset & 1) == 1) {
			length += l.l[i];

			if (length > target)
				return false;
		}

		subset >>= 1;
	}

	return length == target;
}

static inline uint32_t calculate_variants(const Lengths& l, uint8_t n, uint64_t target)
{
	uint32_t variants = 0;
	static vector<map<vector<uint32_t>, uint32_t, LengthsCmp>> cache(20);

	// For sorting, we convert the array to a vector.
	// This is ugly, but faster than using vectors consistently.
	vector<uint32_t> v(n);

	for (uint8_t i = 0; i < n; i++)
		v[i] = l.l[i];

	sort(v.begin(), v.end());
	auto search = cache[n].find(v);

	if (search != cache[n].end())
		return search->second;

	for (uint32_t i = 1; i < (uint32_t) (1 << (n - 1)); i++) {
		if (subset_is_length(l, n, i, target))
			variants++;
	}

	cache[n].insert(make_pair(v, variants));

	return variants;
}

uint32_t generate_solutions(const Lengths& l, uint8_t n, uint64_t total)
{
	uint32_t variants = 0;
	uint64_t halfsize = total / 2;
	uint64_t quartersize = total / 4;
	uint32_t limit = 1 << (n - 1);

	Lengths l1, l2;
	uint8_t n1, n2;

	for (uint32_t half = 3; half < limit; half++) {
		if (!subset_is_length(l, n, half, halfsize))
			continue;

		uint64_t state = half;
		n1 = 0, n2 = 0;

		for (uint8_t i = 0; i < n; i++) {
			if ((state & 1) == 1) {
				l1.l[n1] = l.l[i];
				n1++;
				l2.l[n - n1] = 0;
			} else {
				l2.l[n2] = l.l[i];
				n2++;
				l1.l[n - n2] = 0;
			}

			state >>= 1;
		}

		uint32_t variants1, variants2;

		if (n1 <= n2)
			variants1 = calculate_variants(l1, n1, quartersize);
		else
			variants1 = calculate_variants(l2, n2, quartersize);

		if (variants1 == 0)
			continue;

		if (n1 <= n2)
			variants2 = calculate_variants(l2, n2, quartersize);
		else
			variants2 = calculate_variants(l1, n1, quartersize);

		variants += (variants1 * variants2);
	}

	return variants / 3;
}

void process_test(void)
{
	uint16_t n;

	cin >> n;

	uint64_t total = 0;
	Lengths l;

	for (uint16_t i = 0; i < n; i++) {
		cin >> l.l[i];
		total += l.l[i];
	}

	uint32_t count = generate_solutions(l, n, total);
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
