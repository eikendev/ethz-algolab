#include <cstdlib>
#include <iostream>
#include <numeric>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, uint64_t, boost::property<boost::edge_residual_capacity_t, uint64_t, boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

class edge_adder
{
	graph &g;

public:
	explicit edge_adder(graph &g) : g(g) {}

	void add_edge(int32_t from, int32_t to, uint64_t capacity)
	{
		auto c_map = boost::get(boost::edge_capacity, g);
		auto r_map = boost::get(boost::edge_reverse, g);
		const edge_desc e = boost::add_edge(from, to, g).first;
		const edge_desc rev_e = boost::add_edge(to, from, g).first;
		c_map[e] = capacity;
		c_map[rev_e] = 0;
		r_map[e] = rev_e;
		r_map[rev_e] = e;
	}
};

void process_test(void)
{
	uint16_t ng, ns, l;
	cin >> ng >> ns >> l;

	graph g(ng + ns + 2);
	edge_adder adder(g);

	const uint32_t v_s = ng + ns;
	const uint32_t v_t = v_s + 1;

	for (uint16_t i = 0; i < ng; i++)
		adder.add_edge(v_s, i, 1);

	for (uint16_t i = 0; i < ns; i++)
		adder.add_edge(ng + i, v_t, 1);

	for (uint16_t i = 0; i < l; i++) {
		uint16_t from, to;
		cin >> from >> to;
		adder.add_edge(from, ng + to, 1);
	}

	boost::push_relabel_max_flow(g, v_s, v_t);

	vector<bool> visited(ng + ns + 2, false);
	queue<vertex_desc> bfs;
	visited[v_s] = true;
	bfs.push(v_s);

	while (not bfs.empty()) {
		const auto u = bfs.front();
		bfs.pop();

		for (const auto& e : boost::make_iterator_range(boost::out_edges(u, g))) {
			const auto v = boost::target(e, g);

			if (boost::get(boost::edge_residual_capacity, g, e) == 0 or visited[v])
				continue;

			visited[v] = true;
			bfs.push(v);
		}
	}

	uint16_t g_count = 0;
	uint16_t s_count = 0;

	for (uint16_t i = 0; i < ng; i++)
		if (not visited[i])
			g_count++;

	for (uint16_t i = 0; i < ns; i++)
		if (visited[ng + i])
			s_count++;

	cout << g_count << " " << s_count << endl;

	if (g_count == 0 and s_count == 0)
		return;

	for (uint16_t i = 0; i < ng; i++)
		if (not visited[i])
			cout << i << " ";

	for (uint16_t i = 0; i < ns; i++)
		if (visited[ng + i])
			cout << i << " ";

	cout << endl;
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
