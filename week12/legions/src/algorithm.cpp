#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

using namespace std;

typedef double IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int64_t floor_to_int(const CGAL::Quotient<ET>& x)
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
	int32_t xs, ys;
	uint16_t n;
	cin >> xs >> ys >> n;

	Program lp(CGAL::SMALLER, false, 0, false, 0);

	const uint16_t X = 0;
	const uint16_t Y = 1;
	const uint16_t R = 2;

	lp.set_l(R, true, 0);
	lp.set_c(R, -1);

	for (uint16_t i = 0; i < n; i++) {
		int64_t a, b, c;
		uint16_t v;
		cin >> a >> b >> c >> v;

		IT sqnorm = pow(a, 2) + pow(b, 2);
		IT r_coeff = sqrt(sqnorm);
		assert(pow(r_coeff, 2) == sqnorm);
		r_coeff = r_coeff * v;

		if (a * xs + b * ys + c < 0) {
			a = -a;
			b = -b;
			c = -c;
		}

		lp.set_a(X, i, -a);
		lp.set_a(Y, i, -b);
		lp.set_a(R, i, r_coeff);
		lp.set_b(i, c);
	}

	Solution s = CGAL::solve_linear_program(lp, ET());
	assert(s.solves_linear_program(lp));

	assert(s.is_optimal());
	const auto value = -s.objective_value();
	cout << floor_to_int(value) << endl;
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
