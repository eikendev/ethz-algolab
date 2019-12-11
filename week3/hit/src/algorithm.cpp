#include <cstdlib>
#include <iostream>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Segment_2 S;
typedef K::Point_2 P;
typedef K::Ray_2 R;

void process_test(uint32_t n)
{
	R target;

	int64_t x1, y1, x2, y2;

	cin >> x1 >> y1 >> x2 >> y2;
	target = R(P(x1, y1), P(x2, y2));

	bool has_intersection = false;

	for (uint32_t i = 0; i < n; i++) {
		cin >> x1 >> y1 >> x2 >> y2;

		if (has_intersection)
			continue;

		P p1 = P(x1, y1);
		P p2 = P(x2, y2);
		S segment = S(p1, p2);

		if (CGAL::do_intersect(target, segment)) {
			has_intersection = true;
		}
	}

	if (has_intersection)
		cout << "yes" << endl;
	else
		cout << "no" << endl;
}

int main(void)
{
	ios_base::sync_with_stdio(false);

	uint32_t n;

	cin >> n;

	while (n != 0) {
		process_test(n);
		cin >> n;
	}

	return EXIT_SUCCESS;
}
