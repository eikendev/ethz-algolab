#include <cstdlib>
#include <iostream>

#include <boost/pending/disjoint_sets.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace std;

typedef boost::default_color_type color;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, uint32_t, boost::property<boost::edge_color_t, color>>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

const color black = boost::color_traits<color>::black();
const color white = boost::color_traits<color>::white();

void process_test(void)
{
	uint16_t n, i;
	cin >> n >> i;

	weighted_graph g(n);
	weight_map weights = boost::get(boost::edge_weight, g);

	vector<edge_desc> edges;
	edges.reserve(n * n);

	for (uint16_t j = 0; j < n - 1; j++) {
		for (uint16_t k = 1; k < n - j; k++) {
			uint32_t weight;
			cin >> weight;

			edge_desc e;
			e = boost::add_edge(j, j + k, g).first;
			weights[e] = weight;
			boost::put(boost::edge_color, g, e, color::white_color);
			edges.push_back(e);
		}
	}

	const auto cmp = [&weights](const edge_desc left, const edge_desc right) { return weights[left] < weights[right]; };
	sort(edges.begin(), edges.end(), cmp);

	vector<edge_desc> mst;
	boost::kruskal_minimum_spanning_tree(g, back_inserter(mst));
	uint32_t cost = 0;

	for (const auto& e : mst) {
		cost += weights[e];
		boost::put(boost::edge_color, g, e, color::black_color);
	}

	uint32_t additional_cost = numeric_limits<uint32_t>::max();

	for (const auto& e : mst) {
		boost::disjoint_sets_with_storage<> uf(n);

		for (const auto& e2 : mst)
			if (e != e2)
				uf.link(boost::source(e2, g), boost::target(e2, g));

		for (const auto& e2 : edges) {
			if (boost::get(boost::edge_color, g, e2) == color::white_color) {
				uint32_t c1 = uf.find_set(boost::source(e2, g));
				uint32_t c2 = uf.find_set(boost::target(e2, g));

				if (c1 != c2) {
					additional_cost = min(additional_cost, weights[e2] - weights[e]);
					break;
				}
			}
		}
	}

	cout << cost + additional_cost << endl;
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
