#include <cstdlib>
#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

typedef int IT;
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef struct {
	int32_t x;
	int32_t y;
} point_t;

uint64_t round_to_ceil(const CGAL::Quotient<ET>& x)
{
	double a = std::ceil(CGAL::to_double(x));

	while (a < x) a += 1;
	while (a - 1 >= x) a -= 1;

	return (uint64_t) (a + 0.5);
}

void process_test(void)
{
	uint32_t n;
	uint16_t m, h, w;

	cin >> n >> m >> h >> w;

	Program lp(CGAL::SMALLER, true, 1, false, 0);

	vector<point_t> free_nails(n);

	for (uint32_t i = 0; i < n; i++) {
		cin >> free_nails[i].x;
		cin >> free_nails[i].y;

		lp.set_c(i, -2 * (h + w));
	}

	uint32_t equation = 0;

	for (uint32_t i = 0; i < n; i++) {
		for (uint32_t j = i + 1; j < n; j++) {
			lp.set_a(i, equation, 1);
			lp.set_a(j, equation, 1);

			ET dx = abs(free_nails[i].x - free_nails[j].x);
			ET dy = abs(free_nails[i].y - free_nails[j].y);

			ET limit = 2 * max(dx / w, dy / h);
			lp.set_b(equation, limit);

			equation++;
		}
	}

	vector<point_t> occupied_nails(m);

	for (uint32_t i = 0; i < m; i++) {
		cin >> occupied_nails[i].x;
		cin >> occupied_nails[i].y;
	}

	for (uint32_t i = 0; i < n; i++) {
		lp.set_a(i, equation, 1);

		ET limit = numeric_limits<int64_t>::max();

		for (uint32_t j = 0; j < m; j++) {
			ET dx = abs(free_nails[i].x - occupied_nails[j].x);
			ET dy = abs(free_nails[i].y - occupied_nails[j].y);

			limit = min(limit, 2 * max(dx / w, dy / h));
		}

		lp.set_b(equation, limit - 1);

		equation++;
	}

	Solution s = CGAL::solve_linear_program(lp, ET());
	assert(s.solves_linear_program(lp));
	assert(s.is_optimal());

	cout << round_to_ceil(-s.objective_value()) << endl;
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
