#include <cstdlib>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, uint64_t>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

void process_test(void)
{
	uint16_t n, m, k;
	uint64_t x;
	cin >> n >> m >> x >> k;

	graph g(n);

	for (uint16_t i = 0; i < m; i++) {
		uint16_t u, v;
		uint64_t p;
		cin >> u >> v >> p;

		edge_desc e = boost::add_edge(u, v, g).first;
		boost::put(boost::edge_weight, g, e, p);
	}

	vector<vector<uint64_t>> scores(k + 1, vector<uint64_t>(n, 0));

	bool success = false;
	uint16_t max_moves = 0;

	for (uint16_t i = 1; i <= k; i++) {
		for (uint16_t j = 0; j < n; j++) {
			uint64_t new_sum = 0;

			for (const auto e : boost::make_iterator_range(boost::out_edges(j, g))) {
				uint64_t partial_score = boost::get(boost::edge_weight, g, e);
				partial_score += scores[i - 1][boost::target(e, g)];
				new_sum = max(new_sum, partial_score);
			}

			if (boost::out_degree(j, g) == 0)
				new_sum = scores[i][0];

			scores[i][j] = new_sum;

			if (j == 0 and new_sum >= x) {
				max_moves = i;
				success = true;
				goto post_fill_table;
			}
		}
	}
post_fill_table:

	if (success)
		cout << max_moves << endl;
	else
		cout << "Impossible" << endl;
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
