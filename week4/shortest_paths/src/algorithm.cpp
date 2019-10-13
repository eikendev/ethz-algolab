#include <cstdlib>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

typedef boost::property<boost::edge_weight_t, uint32_t> edge_property;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, edge_property> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

pair<int64_t, bool> dijkstra_dist(const weighted_graph &g, uint32_t s, uint32_t t)
{
	uint32_t n = boost::num_vertices(g);
	std::vector<int64_t> dist_map(n);
	std::vector<vertex_desc> pred_map(n);

	boost::dijkstra_shortest_paths(g, s, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, g))).predecessor_map(boost::make_iterator_property_map(pred_map.begin(), boost::get(boost::vertex_index, g))));

	bool found = (pred_map[t] != t) || (s == t);

	return {dist_map[t], found};
}

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	uint32_t n, m;
	uint16_t q;

	cin >> n >> m >> q;

	weighted_graph g(n);
	weight_map weights = boost::get(boost::edge_weight, g);

	for (uint32_t i = 0; i < n; i++) {
		int32_t lon, lat;

		cin >> lon >> lat;
	}

	for (uint32_t i = 0; i < m; i++) {
		uint32_t a, b, c;

		cin >> a >> b >> c;
		edge_desc e = boost::add_edge(a, b, g).first;
		weights[e] = c;
	}

	for (uint16_t i = 0; i < q; i++) {
		uint32_t s, t;

		cin >> s >> t;

		auto result = dijkstra_dist(g, s, t);

		if (result.second)
			cout << result.first << endl;
		else
			cout << "unreachable" << endl;
	}

	return EXIT_SUCCESS;
}
