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

inline int32_t round_down(const CGAL::Quotient<ET>& x)
{
	int32_t a = (int32_t) std::floor(CGAL::to_double(x));

	while (a > x) a -= 1;
	while (a+1 <= x) a += 1;

	return a;
}

void process_test(uint16_t n, uint16_t m)
{
	Program lp(CGAL::LARGER, true, 0, false, 0);

	lp.set_c0(0);

	for (uint16_t i = 0; i < n; i++) {
		int32_t min_amount, max_amount;
		cin >> min_amount >> max_amount;

		lp.set_b(i, min_amount);
		lp.set_b(i + n, -max_amount);
	}

	for (uint16_t i = 0; i < m; i++) {
		int32_t price;
		cin >> price;
		lp.set_c(i, price);

		for (uint16_t j = 0; j < n; j++) {
			int32_t amount;
			cin >> amount;
			lp.set_a(i, j, amount);
			lp.set_a(i, j + n, -amount);
		}
	}

	Solution s = CGAL::solve_linear_program(lp, ET());
	assert(s.solves_linear_program(lp));

	if (s.is_optimal())
		cout << round_down(s.objective_value()) << endl;
	else
		cout << "No such diet." << endl;
}

int main(void)
{
	ios_base::sync_with_stdio(false);

	uint16_t n, m;

	while (true) {
		cin >> n >> m;

		if (n == 0 or m == 0)
			break;

		process_test(n, m);
	}

	return EXIT_SUCCESS;
}
