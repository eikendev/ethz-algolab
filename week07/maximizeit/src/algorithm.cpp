#include <cassert>
#include <cstdlib>
#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

typedef int32_t IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

static const uint32_t X = 0;
static const uint32_t Y = 1;
static const uint32_t Z = 2;

inline int32_t round_up(const CGAL::Quotient<ET>& x)
{
	int32_t a = (int32_t) std::ceil(CGAL::to_double(x));

	while (a < x) a += 1;
	while (a - 1 >= x) a -= 1;

	return a;
}

inline int32_t round_down(const CGAL::Quotient<ET>& x)
{
	int32_t a = (int32_t) std::floor(CGAL::to_double(x));

	while (a > x) a -= 1;
	while (a+1 <= x) a += 1;

	return a;
}

void solve_program(Program& lp, bool negate, bool do_round_up)
{
	Solution s = CGAL::solve_linear_program(lp, ET());
	assert(s.solves_linear_program(lp));

	if (s.is_infeasible()) {
		cout << "no" << endl;
	} else if (s.is_unbounded()) {
		cout << "unbounded" << endl;
	} else {
		CGAL::Quotient<ET> res = s.objective_value();
		res = (negate) ? -res : res;

		int32_t value = do_round_up ? round_up(res) : round_down(res);
		cout << value << endl;
	}
}

void system1(int16_t a, int16_t b)
{
	Program lp(CGAL::SMALLER, true, 0, false, 0);

	lp.set_c0(0);

	lp.set_c(X, a);
	lp.set_c(Y, -b);

	lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
	lp.set_a(X, 1,  4); lp.set_a(Y, 1, 2); lp.set_b(1, a * b);
	lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);

	solve_program(lp, true, false);
}

void system2(int16_t a, int16_t b)
{
	Program lp(CGAL::LARGER, false, 0, true, 0);

	lp.set_c0(0);

	lp.set_c(X, a);
	lp.set_c(Y, b);
	lp.set_c(Z, 1);

	lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_a(Z, 0, 0); lp.set_b(0, -4);
	lp.set_a(X, 1,  4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -(a * b));
	lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_a(Z, 2, 0); lp.set_b(2, -1);

	solve_program(lp, false, true);
}

int main(void)
{
	ios_base::sync_with_stdio(false);

	uint16_t type;

	while (cin >> type) {
		int16_t a, b;

		cin >> a >> b;

		switch (type) {
		case 1:
			system1(a, b);
			break;
		case 2:
			system2(a, b);
			break;
		default:
			assert(false);
			break;
		}
	};

	return EXIT_SUCCESS;
}
