#include <cstdlib>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, uint32_t>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;

void process_test(void)
{
	uint32_t n, m, k, T;

	cin >> n >> m >> k >> T;

	vector<uint32_t> telenet(T);

	for (uint32_t i = 0; i < T; i++)
		cin >> telenet[i];

	weighted_graph g(n);
	weight_map weights = boost::get(boost::edge_weight, g);
	edge_desc e;

	for (uint32_t i = 0; i < m; i++) {
		uint32_t u, v, c;

		cin >> u >> v >> c;
		e = boost::add_edge(u, v, g).first;
		weights[e] = c;
	}

	vector<uint32_t> scc_map(n);
	uint32_t nscc = boost::strong_components(g, boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, g)));

	weighted_graph g_t(n);
	weight_map weights_t = boost::get(boost::edge_weight, g_t);
	edge_it ebeg, eend;

	for (boost::tie(ebeg, eend) = boost::edges(g); ebeg != eend; ++ebeg) {;
		uint32_t u = boost::source(*ebeg, g);
		uint32_t v = boost::target(*ebeg, g);

		e = boost::add_edge(v, u, g_t).first;
		weights_t[e] = weights[*ebeg];
	}

	vector<int32_t> links_in_scc(nscc, 0);

	for (const auto& v : telenet)
		links_in_scc[scc_map[v]]++;

	vector<vertex_desc> jump_for_scc(nscc);
	vector<bool> scc_has_jump(nscc, false);

	for (const auto& v : telenet) {
		if (!scc_has_jump[scc_map[v]]) {
			jump_for_scc[scc_map[v]] = boost::add_vertex(g_t);
			scc_has_jump[scc_map[v]] = true;
		}

		e = boost::add_edge(v, jump_for_scc[scc_map[v]], g_t).first;
		weights_t[e] = links_in_scc[scc_map[v]] - 1;
		e = boost::add_edge(jump_for_scc[scc_map[v]], v, g_t).first;
		weights_t[e] = 0;
	}

	uint32_t best = numeric_limits<uint32_t>::max();

	vector<uint32_t> dist_map(boost::num_vertices(g_t));
	boost::dijkstra_shortest_paths(g_t, n - 1, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, g_t))));

	for (uint32_t i = 0; i < k; i++)
		best = min(best, dist_map[i]);

	if (best <= 1000000)
		cout << best << endl;
	else
		cout << "no" << endl;
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
