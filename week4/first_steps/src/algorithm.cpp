#include <cstdlib>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, uint16_t>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

uint32_t sum_weights(const weighted_graph& g, const weight_map& weights) {
	std::vector<edge_desc> mst;

	boost::kruskal_minimum_spanning_tree(g, std::back_inserter(mst));

	uint32_t sum = 0;

	for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
		sum += weights[*it];
	}

	return sum;
}

uint16_t max_dist(const weighted_graph& g, uint16_t s) {
	uint16_t n = boost::num_vertices(g);
	std::vector<int> dist_map(n);

	boost::dijkstra_shortest_paths(g, s, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, g))));

	uint16_t distance = *max_element(dist_map.begin(), dist_map.end());

	return distance;
}

void process_test(void)
{
	uint16_t n, m;

	cin >> n >> m;

	weighted_graph g(n - 1);
	weight_map weights = boost::get(boost::edge_weight, g);
	edge_desc e;

	for (uint16_t i = 0; i < m; i++) {
		uint16_t start, end, weight;

		cin >> start >> end >> weight;

		e = boost::add_edge(start, end, g).first;
		weights[e] = weight;
	}

	uint32_t w = sum_weights(g, weights);
	uint16_t d = max_dist(g, 0);

	cout << w << " " << d << endl;
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
