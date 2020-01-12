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
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

class edge_adder
{
	graph &g;

public:
	explicit edge_adder(graph &g) : g(g) {}

	void add_edge(int32_t from, int32_t to, uint64_t capacity, int64_t cost)
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
	cin >> n;

	graph g(n);
	edge_adder adder(g);
	const vertex_desc v_s = n;
	const vertex_desc v_t = v_s + 1;

	for (uint16_t i = 0; i < n; i++) {
		uint16_t a, c;
		cin >> a >> c;
		adder.add_edge(v_s, i, a, c);
	}

	uint64_t students = 0;

	for (uint16_t i = 0; i < n; i++) {
		uint16_t s, p;
		cin >> s >> p;
		students += s;
		adder.add_edge(i, v_t, s, 20 - p);
	}

	for (uint16_t i = 0; i < n - 1; i++) {
		uint16_t v, e;
		cin >> v >> e;
		adder.add_edge(i, i + 1, v, e);
	}

	boost::successive_shortest_path_nonnegative_weights(g, v_s, v_t);
	int64_t cost = boost::find_flow_cost(g);

	uint64_t flow = 0;

	for(const auto& e : boost::make_iterator_range(boost::out_edges(v_s, g)))
		flow += boost::get(boost::edge_capacity, g, e) - boost::get(boost::edge_residual_capacity, g, e);

	cost -= flow * 20;

	if (flow == students)
		cout << "possible ";
	else
		cout << "impossible ";

	cout << flow << " " << -cost << endl;
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
