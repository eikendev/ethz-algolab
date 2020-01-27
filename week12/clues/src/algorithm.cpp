#include <cstdlib>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::no_property> Graph;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<uint32_t, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef CGAL::Delaunay_triangulation_2<K, Tds>::Vertex_handle vertex_handle;
typedef CGAL::Circle_2<K> Circle;

typedef pair<K::Point_2, uint32_t> IPoint;

void process_test(void)
{
	uint32_t n, m;
	uint64_t r;
	cin >> n >> m >> r;

	const K::FT r2 = r * r;

	vector<IPoint> clients;
	clients.reserve(n);

	for (uint32_t i = 0; i < n; ++i) {
		int32_t x, y;
		cin >> x >> y;
		clients.emplace_back(K::Point_2(x, y), i);
	}

	Graph g(n);
	Triangulation t;
	t.insert(clients.begin(), clients.end());

	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++) {
		if (t.segment(e).squared_length() <= r2) {
			uint32_t i1 = e->first->vertex((e->second+1)%3)->info();
			uint32_t i2 = e->first->vertex((e->second+2)%3)->info();
			boost::add_edge(i1, i2, g);
		}
	}

	bool has_inteferences = not is_bipartite(g);

	if (not has_inteferences) {
		for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
			vector<vertex_handle> neighbors;
			auto c = t.incident_edges(v);

			do {
				if (t.is_infinite(c))
					continue;

				const auto vh = c->first->vertex(c->first->ccw(c->second));

				for (const auto& u : neighbors)
					if (CGAL::squared_distance(vh->point(), u->point()) <= r2)
						boost::add_edge(vh->info(), u->info(), g);

				neighbors.push_back(vh);
			} while (++c != t.incident_edges(v));
		}

		has_inteferences = not is_bipartite(g);
	}

	vector<uint32_t> component(n);
	boost::connected_components(g, boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, g)));

	for (uint32_t i = 0; i < m; i++) {
		int32_t x1, y1, x2, y2;
		cin >> x1 >> y1 >> x2 >> y2;

		if (has_inteferences) {
			cout << "n";
			continue;
		}

		const K::Point_2 a(x1, y1);
		const K::Point_2 b(x2, y2);

		const auto nearest_a = t.nearest_vertex(a);
		const auto nearest_b = t.nearest_vertex(b);

		if (CGAL::squared_distance(a, b) <= r2)
			cout << "y";
		else if (CGAL::squared_distance(a, nearest_a->point()) > r2)
			cout << "n";
		else if (CGAL::squared_distance(b, nearest_b->point()) > r2)
			cout << "n";
		else if (component[nearest_a->info()] == component[nearest_b->info()])
			cout << "y";
		else
			cout << "n";
	}

	cout << endl;
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
