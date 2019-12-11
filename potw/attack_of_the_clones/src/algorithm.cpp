#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

typedef struct {
	int32_t start;
	uint32_t pstart;
	uint32_t end;
	bool wraps;
} Protection;

static uint32_t n, m;

typedef priority_queue<uint32_t, vector<uint32_t>, std::greater<uint32_t>> IndexQueue;

static inline uint32_t best_begin(IndexQueue& starts, IndexQueue& ends)
{
	int32_t best_count = 0;
	uint32_t best_index = 0;
	int32_t next_count = 0;
	uint32_t next_index = 0;

	while (not starts.empty() and starts.top() == 0)
		starts.pop();

	while (not ends.empty() and ends.top() == 0)
		ends.pop();

	while (not starts.empty() and not ends.empty()) {
		if (starts.top() == ends.top()) {
			starts.pop();
			ends.pop();
		} else if (starts.top() < ends.top()) {
			next_index = starts.top();
			next_count++;
			starts.pop();
		} else {
			next_index = ends.top();
			next_count--;
			ends.pop();
		}

		if (next_count < best_count) {
			best_count = next_count;
			best_index = next_index;
		}
	}

	return best_index;
}

static inline uint32_t count_protections(const vector<Protection>& protections, uint32_t count, const uint32_t limit, const uint32_t begin)
{
	if (limit == begin)
		return count;

	bool found = count > 0;
	bool has_wrapped = begin < limit;

	int32_t cursor = begin;

	for (const auto& p : protections) {
		if (p.start >= cursor and (not has_wrapped or p.end < limit)) {
			count++;
			cursor = p.end + 1;
			found = true;
		}
	}

	if (has_wrapped)
		return count;

	if (found)
		cursor = cursor - m;
	else
		cursor = -m;

	for (const auto& p : protections) {
		if (p.start >= cursor and p.end < limit) {
			count++;
			cursor = p.end + 1;
			found = true;
			has_wrapped = has_wrapped or p.wraps;
		}
	}

	return count;
}

void process_test(void)
{
	cin >> n >> m;

	vector<Protection> protections(n);

	IndexQueue starts;
	IndexQueue ends;

	for (uint32_t i = 0; i < n; i++) {
		uint32_t a, b;
		Protection& p = protections[i];

		cin >> a >> b;
		a--;
		b--;
		starts.push(a);
		ends.push((b + 1) % m);

		if (a <= b) {
			p.start = a;
			p.pstart = a;
			p.end = b;
			p.wraps = false;
		} else {
			p.start = a - m;
			p.pstart = a;
			p.end = b;
			p.wraps = true;
		}
	}

	sort(protections.begin(), protections.end(), [](const Protection& a, const Protection& b) {
		return (a.end < b.end) or (a.end == b.end and a.start > b.start);
	});

	const int32_t begin = best_begin(starts, ends);
	uint32_t best = numeric_limits<uint32_t>::min();

	{
		uint32_t count = count_protections(protections, 0, begin, (begin + 1) % m);
		best = max(best, count);
	}

	for (const auto& p : protections) {
		if ((not p.wraps and begin >= p.start and begin <= p.end) or (p.wraps and (begin <= p.end or begin >= p.pstart))) {
			uint32_t count = count_protections(protections, 1, p.pstart, (p.end + 1) % m);
			best = max(best, count);
		}
	}

	cout << best << endl;
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
