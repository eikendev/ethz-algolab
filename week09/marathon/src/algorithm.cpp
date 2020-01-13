#include <cstdlib>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, uint64_t, boost::property<boost::edge_residual_capacity_t, uint64_t, boost::property<boost::edge_reverse_t, traits::edge_descriptor, boost::property<boost::edge_weight_t, int64_t>>>>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_capacity_t, uint64_t, boost::property<boost::edge_weight_t, int64_t>>> ugraph;
typedef boost::graph_traits<ugraph>::edge_descriptor uedge_desc;

class edge_adder
{
	graph &g;

public:
	explicit edge_adder(graph &g) : g(g) {}
	void add_edge(vertex_desc from, vertex_desc to, uint64_t capacity, int64_t cost)
	{
		auto c_map = boost::get(boost::edge_capacity, g);
		auto r_map = boost::get(boost::edge_reverse, g);
		auto w_map = boost::get(boost::edge_weight, g);
		const edge_desc e = boost::add_edge(from, to, g).first;
		const edge_desc rev_e = boost::add_edge(to, from, g).first;
		c_map[e] = capacity;
		c_map[rev_e] = 0;
		r_map[e] = rev_e;
		r_map[rev_e] = e;
		w_map[e] = cost;
		w_map[rev_e] = -cost;
	}
};

void process_test(void)
{
	uint16_t n, m, v_s, v_t;
	cin >> n >> m >> v_s >> v_t;

	ugraph g_f(n);
	ugraph g_b(n);

	for (uint16_t i = 0; i < m; i++) {
		uint16_t a, b, c, d;
		cin >> a >> b >> c >> d;

		if (a != b) {
			const uedge_desc e_f = boost::add_edge(a, b, g_f).first;
			boost::put(boost::edge_capacity, g_f, e_f, c);
			boost::put(boost::edge_weight, g_f, e_f, d);
			const uedge_desc e_b = boost::add_edge(b, a, g_b).first;
			boost::put(boost::edge_capacity, g_b, e_b, c);
			boost::put(boost::edge_weight, g_b, e_b, d);
		}
	}

	vector<uint64_t> dist_map_s(n);
	boost::dijkstra_shortest_paths(g_f, v_s, boost::distance_map(boost::make_iterator_property_map(dist_map_s.begin(), boost::get(boost::vertex_index, g_f))));

	vector<uint64_t> dist_map_t(n);
	boost::dijkstra_shortest_paths(g_b, v_t, boost::distance_map(boost::make_iterator_property_map(dist_map_t.begin(), boost::get(boost::vertex_index, g_b))));

	graph g(n);
	edge_adder adder(g);

	for (const auto &e : boost::make_iterator_range(boost::edges(g_f))) {
		const vertex_desc source = boost::source(e, g_f);
		const vertex_desc target = boost::target(e, g_f);
		vertex_desc from, to;
		tie(from, to) = dist_map_s[source] <= dist_map_s[target] ? make_pair(source, target) : make_pair(target, source);

		if (dist_map_s[from] + boost::get(boost::edge_weight, g_f, e) + dist_map_t[to] == dist_map_s[v_t])
			adder.add_edge(from, to, boost::get(boost::edge_capacity, g_f, e), boost::get(boost::edge_weight, g_f, e));
	}

	boost::successive_shortest_path_nonnegative_weights(g, v_s, v_t);
	uint64_t flow = 0;

	for (const auto &e : boost::make_iterator_range(boost::out_edges(boost::vertex(v_s, g), g)))
		flow += boost::get(boost::edge_capacity, g, e) - boost::get(boost::edge_residual_capacity, g, e);

	cout << flow << endl;
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
