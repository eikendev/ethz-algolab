#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#define UPPER_BOUND (30)

using namespace std;

typedef CGAL::Gmpz IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef struct {
	array<int16_t, 3> values;
	array<array<IT, UPPER_BOUND + 1>, 3> powers;
	uint16_t equation;
} Coordinate;

inline void set_equation_parameter(Program& lp, uint16_t d, const vector<Coordinate>& vec, bool positive)
{
	for (const auto& c : vec) {
		if (positive) {
			// P(x) - C >= 0
			lp.set_r(c.equation, CGAL::LARGER);
			lp.set_a(0, c.equation, -1);
		} else {
			// P(x) + C <= 0
			lp.set_r(c.equation, CGAL::SMALLER);
			lp.set_a(0, c.equation, 1);
		}

		lp.set_a(1, c.equation, 1);
		lp.set_b(c.equation, 0);

		uint64_t variable = 2;

		for (uint8_t i = 0; i <= d; i++) {
			for (uint8_t j = 0; i + j <= d; j++) {
				for (uint8_t k = 0; i + j + k <= d; k++) {
					IT value = c.powers[0][i] * c.powers[1][j] * c.powers[2][k];
					lp.set_a(variable, c.equation, value);
					variable++;
				}
			}
		}
	}
}

inline bool is_infeasible(uint16_t d, const vector<Coordinate>& healthy, const vector<Coordinate>& tumor)
{
	Program lp(CGAL::SMALLER, false, 0, false, 0);
	lp.set_c(0, -1);
	lp.set_l(0, true, 1);

	set_equation_parameter(lp, d, healthy, false);
	set_equation_parameter(lp, d, tumor, true);

	CGAL::Quadratic_program_options options;
	options.set_pricing_strategy(CGAL::QP_BLAND);
	Solution s = CGAL::solve_linear_program(lp, ET(), options);
	assert(s.solves_linear_program(lp));

	return s.is_infeasible();
}

tuple<bool, uint16_t> test_polynomial(const vector<Coordinate>& healthy, const vector<Coordinate>& tumor)
{
	if (healthy.size() == 0 or tumor.size() == 0)
		return make_tuple(true, 0);

	uint16_t limit_u = 1;
	uint16_t limit_l = 1;

	while (limit_u <= UPPER_BOUND && is_infeasible(limit_u, healthy, tumor)) {
		limit_l = limit_u;
		limit_u *= 2;
	};

	limit_u = min(limit_u, (uint16_t) UPPER_BOUND);

	do {
		uint16_t depth = (limit_u + limit_l) / 2;
		//cout << "Trying depth=" << depth << " with limit_u=" << limit_u << ", limit_l=" << limit_l << endl;

		if (is_infeasible(depth, healthy, tumor)) {
			//cout << "depth=" << depth << " is insufficient" << endl;
			limit_l = depth + 1;
		} else {
			//cout << "depth=" << depth << " is sufficient" << endl;
			limit_u = depth;
		}
	} while (limit_l < limit_u || limit_l == UPPER_BOUND);

	if (limit_l > UPPER_BOUND)
		return make_tuple(false, 0);
	else
		return make_tuple(true, limit_l);
}

void precalculate_powers(vector<Coordinate>& coordinates)
{
	for (auto& c : coordinates) {
		for (uint8_t i = 0; i < c.values.size(); i++) {
			c.powers[i][0] = 1;

			for (uint8_t j = 1; j <= UPPER_BOUND; j++)
				c.powers[i][j] = c.powers[i][j - 1] * c.values[i];
		}
	}
}

void process_test(void)
{
	uint16_t h, t;

	cin >> h >> t;

	vector<Coordinate> healthy(h);
	vector<Coordinate> tumor(t);

	for (uint16_t i = 0; i < h; i++) {
		cin >> healthy[i].values[0] >> healthy[i].values[1] >> healthy[i].values[2];
		healthy[i].equation = i;
	}

	for (uint16_t i = 0; i < t; i++) {
		cin >> tumor[i].values[0] >> tumor[i].values[1] >> tumor[i].values[2];
		tumor[i].equation = h + i;
	}

	precalculate_powers(healthy);
	precalculate_powers(tumor);

	uint16_t depth;
	bool success;

	tie(success, depth) = test_polynomial(healthy, tumor);

	if (success)
		cout << depth << endl;
	else
		cout << "Impossible!" << endl;
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
