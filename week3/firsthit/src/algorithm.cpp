#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

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
	vector<S> segments;

	int64_t x1, y1, x2, y2;

	cin >> x1 >> y1 >> x2 >> y2;
	target = R(P(x1, y1), P(x2, y2));

	S best_segment;
	bool found = false;

	for (uint32_t i = 0; i < n; i++) {
		cin >> x1 >> y1 >> x2 >> y2;

		P p1 = P(x1, y1);
		P p2 = P(x2, y2);
		S segment = S(p1, p2);

		if (CGAL::do_intersect(target, segment))
			segments.push_back(segment);
	}

	random_shuffle(segments.begin(), segments.end());

	for (uint32_t i = 0; i < segments.size(); i++) {
		S segment = segments[i];

		if (found ? CGAL::do_intersect(best_segment, segment) : CGAL::do_intersect(target, segment)) {
			auto o = found ? CGAL::intersection(best_segment, segment) : CGAL::intersection(target, segment);

			if (const P *new_intersection = boost::get<P>(&*o)) {
				best_segment = S(target.source(), *new_intersection);
			} else if (const S *new_intersection = boost::get<S>(&*o)) {
				K::FT ds = CGAL::squared_distance(new_intersection->source(), target.source());
				K::FT dt = CGAL::squared_distance(new_intersection->target(), target.source());

				if (ds < dt)
					best_segment = S(target.source(), new_intersection->source());
				else
					best_segment = S(target.source(), new_intersection->target());
			} else {
				throw std::runtime_error("Invalid segment intersection.");
			}

			found = true;

			if (best_segment.target() == target.source())
				break;
		}
	}

	if (found) {
		int64_t x = floor_to_double(best_segment.target().x());
		int64_t y = floor_to_double(best_segment.target().y());
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
