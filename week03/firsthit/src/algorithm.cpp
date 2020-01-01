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

void print_point(const P& p)
{
	int64_t x = floor_to_double(p.x());
	int64_t y = floor_to_double(p.y());

	cout << x << " " << y << endl;
}

void process_test(uint32_t n)
{
	R target;
	vector<S> segments;

	int64_t x1, y1, x2, y2;

	cin >> x1 >> y1 >> x2 >> y2;
	target = R(P(x1, y1), P(x2, y2));

	for (uint32_t i = 0; i < n; i++) {
		cin >> x1 >> y1 >> x2 >> y2;

		P p1 = P(x1, y1);
		P p2 = P(x2, y2);
		S segment = S(p1, p2);

		segments.push_back(segment);
	}

	if (segments.size() == 0) {
		cout << "no" << endl;
		return;
	}

	random_shuffle(segments.begin(), segments.end());

	S best;
	bool found = false;

	for (const auto& segment : segments) {
		if (found ? CGAL::do_intersect(best, segment) : CGAL::do_intersect(target, segment)) {
			P new_limit;
			auto o = found ? CGAL::intersection(best, segment) : CGAL::intersection(target, segment);

			if (const P *new_intersection = boost::get<P>(&*o)) {
				new_limit = *new_intersection;
			} else if (const S *new_intersection = boost::get<S>(&*o)) {
				if (CGAL::has_smaller_distance_to_point(target.source(), new_intersection->source(), new_intersection->target()))
					new_limit = new_intersection->source();
				else
					new_limit = new_intersection->target();
			} else {
				throw std::runtime_error("Invalid segment intersection.");
			}

			best = S(target.source(), new_limit);
			found = true;

			if (best.target() == target.source())
				break;
		}
	}

	if (found)
		print_point(best.target());
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
