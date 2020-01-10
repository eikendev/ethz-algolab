#include <cstdlib>
#include <iostream>
#include <queue>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/properties.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

enum Color { Black = 0, White = 1, Red = 2 };
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<tuple<Color, uint64_t, double>, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Finite_faces_iterator Finite_faces_iterator;
typedef Triangulation::Face_handle Face_handle;

struct vertex_face_t {
	typedef boost::vertex_property_tag kind;
};

typedef boost::default_color_type color;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<vertex_face_t, Face_handle>, boost::property<boost::edge_weight_t, double, boost::property<boost::edge_color_t, color>>> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

void calculate_escape_widths(Triangulation& t)
{
	Face_handle fh;
	uint64_t n = 0;

	for (Finite_faces_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); f++) {
		get<0>(f->info()) = White;
		get<1>(f->info()) = n++;
		fh = f;
	}

	weighted_graph g(n + 1);
	queue<Face_handle> q;
	q.push(fh);
	get<0>(fh->info()) = Red;
	boost::put(vertex_face_t(), g, get<1>(fh->info()), fh);

	while (not q.empty()) {
		fh = q.front();
		q.pop();
		get<0>(fh->info()) = Black;
		double infinite_width = 0;

		for (uint8_t i = 0; i < 3; i++) {
			Face_handle neighbor = fh->neighbor(i);

			if (get<0>(neighbor->info()) == Black and not t.is_infinite(neighbor))
				continue;

			double width = t.segment(fh, i).squared_length() / 4; // r ≥ 2√d → r²/4 ≥ d

			if (t.is_infinite(neighbor)) {
				infinite_width = max(infinite_width, width);
				continue;
			}

			edge_desc e = boost::add_edge(get<1>(fh->info()), get<1>(neighbor->info()), g).first;
			boost::put(boost::edge_weight, g, e, -width);

			if (get<0>(neighbor->info()) == White) {
				q.push(neighbor);
				get<0>(neighbor->info()) = Red;
				boost::put(vertex_face_t(), g, get<1>(neighbor->info()), neighbor);
			}
		}

		if (infinite_width > 0) {
			edge_desc e = boost::add_edge(get<1>(fh->info()), n, g).first;
			boost::put(boost::edge_weight, g, e, -infinite_width);
		}
	}

	vector<edge_desc> vmst;
	vmst.reserve(n);
	boost::kruskal_minimum_spanning_tree(g, back_inserter(vmst));
	weighted_graph mst(n + 1);

	for (const auto& e : vmst) {
		edge_desc e2 = boost::add_edge(boost::source(e, g), boost::target(e, g), mst).first;
		boost::put(boost::edge_weight, mst, e2, -boost::get(boost::edge_weight, g, e));
		boost::put(boost::edge_color, mst, e2, boost::color_traits<color>::white());
	}

	for (const auto& v : boost::make_iterator_range(boost::vertices(g)))
		boost::put(vertex_face_t(), mst, v, boost::get(vertex_face_t(), g, v));

	vertex_desc vd = n;
	queue<tuple<vertex_desc, double>> bfs;
	bfs.push(make_tuple(vd, numeric_limits<double>::max()));

	while (not bfs.empty()) {
		double width;
		tie(vd, width) = bfs.front();
		bfs.pop();

		for (const auto& e : boost::make_iterator_range(boost::out_edges(vd, mst))) {
			if (boost::get(boost::edge_color, mst, e) == boost::color_traits<color>::black())
				continue;

			boost::put(boost::edge_color, mst, e, boost::color_traits<color>::black());
			const auto to = boost::target(e, mst);
			assert(to != vd);
			double new_width = min(width, boost::get(boost::edge_weight, mst, e));
			get<2>(boost::get(vertex_face_t(), mst, to)->info()) = new_width;
			bfs.push(make_tuple(to, new_width));
		}
	}
}

bool can_escape(const Triangulation& t, const int32_t x, const int32_t y, const uint64_t d)
{
	const K::Point_2 p = K::Point_2(x, y);

	if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) < (double) d)
		return false;

	Face_handle fh = t.locate(p);

	if (t.is_infinite(fh))
		return true;

	return get<2>(fh->info()) >= (double) d;
}

void process_test(const uint32_t n)
{
	vector<K::Point_2> points;
	points.reserve(n);

	for (uint32_t i = 0; i < n; i++) {
		int32_t x, y;
		cin >> x >> y;
		points.push_back(K::Point_2(x, y));
	}

	Triangulation t;
	t.insert(points.begin(), points.end());
	calculate_escape_widths(t);

	uint32_t m;
	cin >> m;

	for (uint32_t i = 0; i < m; i++) {
		int32_t x, y;
		uint64_t d;
		cin >> x >> y >> d;

		if (can_escape(t, x, y, d))
			cout << "y";
		else
			cout << "n";
	}

	cout << endl;
}

int main(void)
{
	ios_base::sync_with_stdio(false);

	uint32_t n;
	cin >> n;

	while (n != 0) {
		process_test(n);
		cin >> n;
	}

	return EXIT_SUCCESS;
}
