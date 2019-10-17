#include <cstdlib>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/properties.hpp>

using namespace std;

typedef boost::property<boost::edge_weight_t, uint32_t, boost::property<boost::edge_color_t, boost::default_color_type>> edge_property;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, edge_property> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::default_color_type color;

const color white = boost::color_traits<color>::white();
const color black = boost::color_traits<color>::black();

uint64_t min_weight(const weighted_graph &g, uint16_t s, uint16_t t) {
	size_t n = boost::num_vertices(g);
	std::vector<vertex_desc> dist_map(n);

	boost::dijkstra_shortest_paths(g, s, distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, g))));

	return dist_map[t];
}

void process_test(void) {
	edge_desc edge;
	uint16_t n, s, a, b;
	uint32_t e;

	cin >> n >> e >> s >> a >> b;

	vector<weighted_graph> g(s, weighted_graph(n));

	for (uint32_t i = 0; i < e; i++) {
		uint16_t start, end;
		uint32_t weight;

		cin >> start >> end;

		for (uint16_t j = 0; j < s; j++) {
			cin >> weight;
			edge = boost::add_edge(start, end, g[j]).first;
			boost::put(boost::edge_weight, g[j], edge, weight);
		}
	}

	for (uint16_t i = 0; i < s; i++) {
		vertex_desc hive;

		cin >> hive;

		for (auto e : boost::make_iterator_range(boost::edges(g[i])))
			boost::put(boost::edge_color, g[i], e, white);

		vector<edge_desc> mst;
		boost::kruskal_minimum_spanning_tree(g[i], std::back_inserter(mst));

		for (auto e : mst)
			boost::put(boost::edge_color, g[i], e, black);
	}

	weighted_graph g_final(n);

	for (auto e : boost::make_iterator_range(boost::edges(g[0]))) {
		uint32_t weight = numeric_limits<uint32_t>::max();
		bool available = false;

		vertex_desc vs = boost::source(e, g[0]);
		vertex_desc vt = boost::target(e, g[0]);

		for (uint16_t i = 0; i < s; i++) {
			edge = boost::edge(vs, vt, g[i]).first;

			if (boost::get(boost::edge_color, g[i], edge) == black) {
				weight = min(weight, boost::get(boost::edge_weight, g[i], edge));
				available = true;
			}
		}

		if (available) {
			edge = boost::add_edge(vs, vt, g_final).first;
			boost::put(boost::edge_weight, g_final, edge, weight);
		}
	}

	uint64_t sum_weight = min_weight(g_final, a, b);

	cout << sum_weight << endl;
}

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	uint16_t t;

	cin >> t;

	for (uint16_t i = 0; i < t; i++)
		process_test();

	return EXIT_SUCCESS;
}
