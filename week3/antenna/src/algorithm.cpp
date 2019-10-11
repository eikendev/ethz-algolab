#include <cstdlib>
#include <iostream>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef K::Point_2 P;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> MinCircle;

double ceil_to_double(const K::FT& x)
{
	double a = std::ceil(CGAL::to_double(x));

	while (a < x) a += 1;
	while (a - 1 >= x) a -= 1;

	return a;
}

void process_test(uint32_t n)
{
	vector<P> points(n);

	for (uint32_t i = 0; i < n; i++) {
		int64_t x, y;

		cin >> x >> y;
		points[i] = P(x, y);
	}

	MinCircle mc(points.begin(), points.end(), true);
	Traits::Circle c = mc.circle();

	uint64_t radius = ceil_to_double(CGAL::sqrt(c.squared_radius()));

	cout << radius << endl;
}

int main(int argc, char const *argv[])
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
