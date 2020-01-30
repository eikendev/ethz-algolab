#include <cstdlib>
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef int64_t IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator Edge_iterator;

int64_t floor_to_int(const CGAL::Quotient<ET> &x)
{
	double a = floor(CGAL::to_double(x));

	while (a > x)
		a -= 1;
	while (a + 1 <= x)
		a += 1;

	return (int64_t) a;
}

void process_test(void)
{
	uint16_t n, m;
	uint32_t c;
	cin >> n >> m >> c;

	Program lp(CGAL::SMALLER, true, 0, false, 0);

	vector<K::Point_2> warehouses;
	warehouses.reserve(n);

	for (uint16_t i = 0; i < n; i++) {
		int32_t x, y;
		uint32_t s;
		uint16_t a;
		cin >> x >> y >> s >> a;

		warehouses.emplace_back(x, y);
		lp.set_b(i, s);

		for (uint16_t j = 0; j < m; j++) {
			lp.set_a(i * m + j, i, 1);
			lp.set_a(i * m + j, n + 2 * j, 1);
			lp.set_a(i * m + j, n + 2 * j + 1, a);
		}
	}

	vector<K::Point_2> stadiums;
	stadiums.reserve(m);

	for (uint16_t i = 0; i < m; i++) {
		int32_t x, y;
		uint32_t d, u;
		cin >> x >> y >> d >> u;

		stadiums.emplace_back(x, y);
		lp.set_b(n + 2 * i, d);
		lp.set_r(n + 2 * i, CGAL::EQUAL);
		lp.set_b(n + 2 * i + 1, 100 * u);
	}

	vector<vector<int32_t>> revenues(n, vector<int32_t>(m));

	for (uint16_t i = 0; i < n; i++) {
		for (uint16_t j = 0; j < m; j++) {
			cin >> revenues[i][j];
			revenues[i][j] = revenues[i][j] * 100;
		}
	}

	Triangulation t;
	t.insert(warehouses.begin(), warehouses.end());
	t.insert(stadiums.begin(), stadiums.end());

	for (uint32_t i = 0; i < c; i++) {
		int32_t x, y;
		uint32_t r;
		cin >> x >> y >> r;
		r = r * r;
		const K::Point_2 center(x, y);

		if (CGAL::squared_distance(center, t.nearest_vertex(center)->point()) > r)
			continue;

		for (uint16_t j = 0; j < n; j++) {
			const auto d1 = CGAL::squared_distance(center, warehouses[j]) <= r;
			for (uint16_t k = 0; k < m; k++) {
				const auto d2 = CGAL::squared_distance(center, stadiums[k]) <= r;

				if (d1 != d2)
					revenues[j][k]--;
			}
		}
	}

	for (uint16_t i = 0; i < n; i++)
		for (uint16_t j = 0; j < m; j++)
			lp.set_c(i * m + j, -revenues[i][j]);

	Solution s = CGAL::solve_linear_program(lp, ET());
	assert(s.solves_linear_program(lp));

	const auto value = floor_to_int(-s.objective_value() / 100);

	if (s.is_optimal())
		cout << value << endl;
	else
		cout << "RIOT!" << endl;
}

int main(void)
{
	ios_base::sync_with_stdio(false);

	uint32_t t;

	cin >> t;

	for (uint32_t i = 0; i < t; i++)
		process_test();

	return EXIT_SUCCESS;
}
