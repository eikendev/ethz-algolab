#include <cstdlib>
#include <iostream>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Segment_2 S;
typedef K::Point_2 P;
typedef K::Ray_2 R;

double floor_to_double(const K::FT& x)
{
	double a = std::floor(CGAL::to_double(x));

	while (a > x) a -= 1;
	while (a + 1 <= x) a += 1;

	return a;
}

void process_testcase(uint32_t n)
{
	R target;

	int64_t x1, y1, x2, y2;

	cin >> x1 >> y1 >> x2 >> y2;
	target = R(P(x1, y1), P(x2, y2));

	P best_intersection;
	K::FT best_distance;
	bool found = false;

	for (uint32_t i = 0; i < n; i++) {
		cin >> x1 >> y1 >> x2 >> y2;

		P p1 = P(x1, y1);
		P p2 = P(x2, y2);
		S segment = S(p1, p2);

		if (CGAL::do_intersect(target, segment)) {
			auto o = CGAL::intersection(target, segment);

			if (const P *new_intersection = boost::get<P>(&*o)) {
				K::FT new_distance;

				new_distance = CGAL::squared_distance(*new_intersection, target.source());

				if (!found || new_distance < best_distance) {
					best_intersection = P(*new_intersection);
					best_distance = new_distance;
					found = true;
				}
			} else if (const S *new_intersection = boost::get<S>(&*o)) {
				K::FT new_distance;

				new_distance = CGAL::squared_distance(new_intersection->source(), target.source());

				if (!found || new_distance < best_distance) {
					best_intersection = P(new_intersection->source());
					best_distance = new_distance;
					found = true;
				}

				new_distance = CGAL::squared_distance(new_intersection->target(), target.source());

				if (!found || new_distance < best_distance) {
					best_intersection = P(new_intersection->target());
					best_distance = new_distance;
					found = true;
				}
			} else {
				throw std::runtime_error("Invalid segment intersection.");
			}
		}
	}

	if (found) {
		int64_t x = floor_to_double(best_intersection.x());
		int64_t y = floor_to_double(best_intersection.y());
		cout << x << " " << y << endl;
	} else {
		cout << "no" << endl;
	}
}

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	uint32_t n;

	cin >> n;

	while (n != 0) {
		process_testcase(n);
		cin >> n;
	}

	return EXIT_SUCCESS;
}
