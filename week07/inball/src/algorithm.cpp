#include <cassert>
#include <cstdlib>
#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

inline int32_t round_down(const CGAL::Quotient<ET>& x)
{
	int32_t a = std::floor(CGAL::to_double(x));

	while (a > x) a -= 1;
	while (a + 1 <= x) a += 1;

	return a;
}

void process_test(uint16_t n, uint16_t d)
{
	Program lp(CGAL::SMALLER, false, 0, false, 0);

	lp.set_c(d, -1);
	lp.set_l(d, true, 0);

	for (uint16_t i = 0; i < n; i++) {
		uint64_t norm = 0;

		for (uint16_t j = 0; j < d; j++) {
			int16_t a;
			cin >> a;
			lp.set_a(j, i, a);

			norm += a * a;
		}

		norm = sqrt(norm);
		lp.set_a(d, i, norm);

		int16_t b;
		cin >> b;
		lp.set_b(i, b);
	}

	Solution s = CGAL::solve_linear_program(lp, ET());
	assert(s.solves_linear_program(lp));

	switch (s.status()) {
	case CGAL::QP_OPTIMAL:
		cout << round_down(-s.objective_value()) << endl;
		break;
	case CGAL::QP_UNBOUNDED:
		cout << "inf" << endl;
		break;
	case CGAL::QP_INFEASIBLE:
		cout << "none" << endl;
		break;
	default:
		assert(false);
	}
}

int main(void)
{
	ios_base::sync_with_stdio(false);

	uint16_t n, d;
	cin >> n;

	while (n != 0) {
		cin >> d;
		process_test(n, d);

		cin >> n;
	}

	return EXIT_SUCCESS;
}
