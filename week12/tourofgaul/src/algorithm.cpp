#include <cstdlib>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, uint64_t, boost::property<boost::edge_residual_capacity_t, uint64_t, boost::property<boost::edge_reverse_t, traits::edge_descriptor, boost::property<boost::edge_weight_t, int64_t>>>>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

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
	uint16_t n;
	uint32_t m;
	cin >> n >> m;

	graph g(n + 2);
	edge_adder adder(g);

	const vertex_desc v_source = n;
	const vertex_desc v_target = v_source + 1;

	const uint16_t max_capacity = 100;
	const uint16_t max_cost = 128;
	uint32_t carrysum = 0;

	adder.add_edge(v_source, 0, max_capacity, 0);
	adder.add_edge(n - 1, v_target, max_capacity, 0);

	for (uint16_t i = 0; i < n - 1; i++) {
		uint16_t c;
		cin >> c;
		carrysum += c;
		adder.add_edge(i, i + 1, c, max_cost);
		adder.add_edge(i, i + 1, max_capacity - c, 0);
	}

	for (uint32_t i = 0; i < m; i++) {
		uint16_t a, b, d;
		cin >> a >> b >> d;
		uint16_t cost = (b - a) * max_cost - d;
		adder.add_edge(a, b, 1, cost);
	}

	boost::successive_shortest_path_nonnegative_weights(g, v_source, v_target);
	int64_t cost = boost::find_flow_cost(g);
	cost = cost - carrysum * max_cost;
	cost = -cost;

	cout << cost << endl;
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
