#include <cstdlib>
#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef uint32_t Index;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

int64_t ceil_to_int(const K::FT& x)
{
	double a = std::floor(CGAL::to_double(x));

	while (a < x)
		a += 1;
	while (a - 1 >= x)
		a -= 1;

	return a;
}

void process_test(Index n)
{
	vector<K::Point_2> points;
	points.reserve(n);

	for (Index i = 0; i < n; i++) {
		int32_t x, y;
		cin >> x >> y;
		points.push_back(K::Point_2(x, y));
	}

	Triangulation t;
	t.insert(points.begin(), points.end());

	Index m;
	cin >> m;

	for (Index i = 0; i < m; i++) {
		int32_t x, y;
		cin >> x >> y;

		const K::Point_2 p1 = K::Point_2(x, y);
		const K::Point_2 p2 = t.nearest_vertex(p1)->point();
		K::FT distance = CGAL::squared_distance(p1, p2);
		cout << ceil_to_int(distance) << endl;
	}
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
