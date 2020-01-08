#include <cstdlib>
#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef uint32_t Index;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;

void process_test(Index n)
{
	vector<K::Point_2> points;
	points.reserve(n);

	for (uint32_t i = 0; i < n; i++) {
		int32_t x, y;
		cin >> x >> y;
		points.push_back(K::Point_2(x, y));
	}

	Triangulation t;
	t.insert(points.begin(), points.end());

	K::FT distance = numeric_limits<uint64_t>::max();

	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
		distance = min(distance, t.segment(e).squared_length());

	cout << ceil(CGAL::sqrt(distance) * 100 / 2) << endl;
}

int main(void)
{
	ios_base::sync_with_stdio(false);

	Index n;
	cin >> n;

	while (n != 0) {
		process_test(n);
		cin >> n;
	}

	return EXIT_SUCCESS;
}
