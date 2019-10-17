#include <cstdlib>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::no_property> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

inline bool count_common_items(const vector<uint32_t>& s1, const vector<uint32_t>& s2, uint32_t pool_size, uint16_t threshold)
{
	vector<bool> pool(pool_size, false);

	uint16_t c = 0;

	for (auto v : s1)
		pool[v] = true;

	for (auto v : s2) {
		if (pool[v] == true) {
			c++;

			if (c == threshold)
				return true;
		}
	}

	return false;
}

void process_test(void)
{
	uint16_t n, c, f;

	cin >> n >> c >> f;

	vector<vector<uint32_t>> cxs(n);
	map<string, uint32_t> cx2id;

	for (uint16_t i = 0; i < n; i++) {
		for (uint16_t j = 0; j < c; j++) {
			string cx;

			cin >> cx;

			uint32_t id;

			id = cx2id.insert({cx, cx2id.size()}).first->second;
			cxs[i].push_back(id);
		}
	}

	graph g(n);

	edge_desc e;

	for (uint16_t i = 0; i < n; i++) {
		for (uint16_t j = 0; j < i; j++) {
			bool share = count_common_items(cxs[i], cxs[j], cx2id.size(), f + 1);

			if (share)
				e = boost::add_edge(i, j, g).first;
		}
	}

	vector<vertex_desc> mate(n);
	bool success = checked_edmonds_maximum_cardinality_matching(g, &mate[0]);
	assert(success);

	uint16_t s = matching_size(g, &mate[0]);

	if (s == n / 2)
		cout << "not optimal" << endl;
	else
		cout << "optimal" << endl;
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
