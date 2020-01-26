#include <cstdlib>
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

typedef double IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef struct {
	K::Point_2 p;
	uint16_t d;
} Particle;

void process_test(void)
{
	uint16_t a, s;
	uint32_t b, e;
	cin >> a >> s >> b >> e;

	vector<Particle> particles;
	particles.reserve(a);

	for (uint16_t i = 0; i < a; i++) {
		int32_t x, y;
		uint16_t d;
		cin >> x >> y >> d;
		particles.push_back({K::Point_2(x, y), d});
	}

	vector<K::Point_2> shots;
	shots.reserve(s);

	for (uint16_t i = 0; i < s; i++) {
		int32_t x, y;
		cin >> x >> y;
		shots.emplace_back(x, y);
	}

	vector<K::Point_2> hunters;
	hunters.reserve(b);

	for (uint32_t i = 0; i < b; i++) {
		int32_t x, y;
		cin >> x >> y;
		hunters.emplace_back(x, y);
	}

	Triangulation t;
	t.insert(hunters.begin(), hunters.end());
	vector<K::FT> radii(s, numeric_limits<K::FT>::max());
	Program lp(CGAL::LARGER, true, 0, false, 0);

	for (auto i = 0; i < s; i++) {
		lp.set_c(i, 1);

		if (b > 0) {
			const auto nearest = t.nearest_vertex(shots[i]);
			radii[i] = CGAL::squared_distance(nearest->point(), shots[i]);
		}
	}

	for (auto i = 0; i < a; i++) {
		lp.set_b(i, particles[i].d);

		for (auto j = 0; j < s; j++) {
			IT factor = 0;
			const auto distance = CGAL::squared_distance(particles[i].p, shots[j]);

			if (distance < radii[j])
				factor = 1 / max(1., distance);

			lp.set_a(j, i, factor);
		}
	}

	Solution solution = CGAL::solve_linear_program(lp, ET());
	assert(solution.solves_linear_program(lp));

	if (solution.is_optimal() and solution.objective_value() <= ET((long) e))
		cout << "y" << endl;
	else
		cout << "n" << endl;
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
