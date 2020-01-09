#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef uint32_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

typedef tuple<Index, Index, K::FT> Edge;
typedef vector<Edge> EdgeV;
typedef pair<K::Point_2, Index> IPoint;

void limit_by_neighborhood(vector<double>& distances, vector<IPoint>& points)
{
	Delaunay t;
	t.insert(points.begin(), points.end());

	EdgeV edges;
	edges.reserve(3 * points.size());
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++) {
		Index i1 = e->first->vertex((e->second + 1) % 3)->info();
		Index i2 = e->first->vertex((e->second + 2) % 3)->info();

		if (i1 > i2)
			std::swap(i1, i2);

		edges.emplace_back(i1, i2, t.segment(e).squared_length());
	}

	std::sort(edges.begin(), edges.end(), [](const Edge &e1, const Edge &e2) -> bool {
		return std::get<2>(e1) < std::get<2>(e2);
	});

	boost::disjoint_sets_with_storage<> uf(points.size());
	Index n_components = points.size();

	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); e++) {
		Index c1 = uf.find_set(std::get<0>(*e));
		Index c2 = uf.find_set(std::get<1>(*e));

		if (c1 != c2) {
			uf.link(c1, c2);
			n_components--;

			double distance = get<2>(*e);
			distances[get<0>(*e)] = min(distances[get<0>(*e)], distance);
			distances[get<1>(*e)] = min(distances[get<1>(*e)], distance);

			if (n_components == 1)
				break;
		}
	}

	for (Index i = 0; i < distances.size(); i++)
		distances[i] = CGAL::sqrt(distances[i]) / 2 - 0.5;
}

void limit_by_dish(vector<double>& distances, vector<IPoint>& points, int32_t x1, int32_t x2, int32_t y1, int32_t y2)
{
	for (Index i = 0; i < distances.size(); i++) {
		distances[i] = min(distances[i], -x1 + get<0>(points[i]).x());
		distances[i] = min(distances[i],  x2 - get<0>(points[i]).x());
		distances[i] = min(distances[i], -y1 + get<0>(points[i]).y());
		distances[i] = min(distances[i],  y2 - get<0>(points[i]).y());
	}
}

uint64_t calculate_distance(vector<double>& distances, Index idx)
{
	uint64_t value = 0;

	if (distances[idx] > 0)
		value = ceil(CGAL::sqrt(distances[idx]));

	return value;
}

void process_test(Index n)
{
	int32_t x1, y1, x2, y2;
	cin >> x1 >> y1 >> x2 >> y2;

	vector<IPoint> points;
	points.reserve(n);

	for (Index i = 0; i < n; i++) {
		int32_t x, y;
		std::cin >> x >> y;
		points.emplace_back(K::Point_2(x, y), i);
	}

	vector<double> distances(n, numeric_limits<double>::max());

	limit_by_neighborhood(distances, points);
	limit_by_dish(distances, points, x1, x2, y1, y2);

	sort(distances.begin(), distances.end());

	uint64_t f = calculate_distance(distances, 0);
	uint64_t m = calculate_distance(distances, (distances.size() + 0.5) / 2);
	uint64_t l = calculate_distance(distances, distances.size() - 1);

	cout << f << " " << m << " " << l << endl;
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
