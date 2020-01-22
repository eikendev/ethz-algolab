#include <cstdlib>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/boost/graph/graph_traits_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/pending/disjoint_sets.hpp>
#include <boost/graph/filtered_graph.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef uint32_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

template <typename T>
struct Is_finite {
	const T *t_;
	Is_finite() : t_(NULL) {
	}
	Is_finite(const T &t) : t_(&t) {
	}
	template <typename VertexOrEdge>
	bool operator()(const VertexOrEdge &voe) const {
		return !t_->is_infinite(voe);
	}
};

typedef Is_finite<Triangulation> Filter;
typedef boost::filtered_graph<Triangulation, Filter, Filter> Finite_triangulation;
typedef boost::graph_traits<Finite_triangulation>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Finite_triangulation>::vertex_iterator vertex_iterator;
typedef boost::graph_traits<Finite_triangulation>::edge_descriptor edge_descriptor;

int64_t ceil_to_int(const K::FT &x)
{
	double a = std::floor(CGAL::to_double(x));

	while (a < x)
		a += 1;
	while (a - 1 >= x)
		a -= 1;

	return (int64_t)a;
}

void process_test(void)
{
	uint16_t n, m;
	uint64_t p;
	cin >> n >> m >> p;

	vector<pair<K::Point_2, Index>> jammers;
	jammers.reserve(n);

	for (uint16_t i = 0; i < n; i++) {
		int32_t x, y;
		cin >> x >> y;
		jammers.emplace_back(K::Point_2(x, y), i);
	}

	Triangulation t;
	Filter is_finite(t);
	Finite_triangulation ft(t, is_finite, is_finite);
	t.insert(jammers.begin(), jammers.end());

	typedef map<vertex_descriptor, uint32_t> VertexIndexMap;
	VertexIndexMap vertex_id_map;
	typedef boost::associative_property_map<VertexIndexMap> VertexIdPropertyMap;
	VertexIdPropertyMap vertex_index_pmap(vertex_id_map);

	for (auto vd = t.finite_vertices_begin(); vd != t.finite_vertices_end(); vd++)
		vertex_id_map[vd] = vd->info();

	vector<edge_descriptor> mst;
	boost::kruskal_minimum_spanning_tree(ft, std::back_inserter(mst), vertex_index_map(vertex_index_pmap));
	vector<pair<edge_descriptor, K::FT>> weights(mst.size());
	boost::disjoint_sets_with_storage<> ufp(n);

	for (uint32_t i = 0; i < mst.size(); i++) {
		const auto from = boost::source(mst[i], ft)->point();
		const auto to = boost::target(mst[i], ft)->point();
		weights[i] = make_pair(mst[i], CGAL::squared_distance(from, to));

		if (weights[i].second <= K::FT(p))
			ufp.link(boost::source(mst[i], ft)->info(), boost::target(mst[i], ft)->info());
	}

	std::sort(weights.begin(), weights.end(), [](const pair<edge_descriptor, K::FT> a, const pair<edge_descriptor, K::FT> b) {
		return a.second < b.second;
	});

	K::FT a = 0, b = 0;
	boost::disjoint_sets_with_storage<> ufa(n);
	boost::disjoint_sets_with_storage<> ufb(n);
	auto iter_a = weights.begin();
	auto iter_b = weights.begin();

	for (uint16_t i = 0; i < m; i++) {
		int32_t x1, y1, x2, y2;
		cin >> x1 >> y1 >> x2 >> y2;
		const auto from = K::Point_2(x1, y1);
		const auto to = K::Point_2(x2, y2);
		const auto from_nearest = t.nearest_vertex(from);
		const auto to_nearest = t.nearest_vertex(to);
		const auto from_distance = CGAL::squared_distance(from, from_nearest->point());
		const auto to_distance = CGAL::squared_distance(to, to_nearest->point());
		const auto max_distance = max(from_distance, to_distance);

		a = max(a, max_distance);

		if (4 * max_distance <= K::FT(p) and ufp.find_set(from_nearest->info()) == ufp.find_set(to_nearest->info())) {
			cout << "y";
			b = max(b, max_distance);

			while (iter_b != weights.end() and ufb.find_set(from_nearest->info()) != ufb.find_set(to_nearest->info())) {
				const auto &e = *iter_b;
				iter_b++;
				b = max(b, e.second / 4);
				ufb.link(boost::source(e.first, ft)->info(), boost::target(e.first, ft)->info());
			}
		} else {
			cout << "n";
		}

		while (iter_a != weights.end() and ufa.find_set(from_nearest->info()) != ufa.find_set(to_nearest->info())) {
			const auto &e = *iter_a;
			iter_a++;
			a = max(a, e.second / 4);
			ufa.link(boost::source(e.first, ft)->info(), boost::target(e.first, ft)->info());
		}
	}

	cout << endl << ceil_to_int(4 * a) << endl << ceil_to_int(4 * b) << endl;
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
