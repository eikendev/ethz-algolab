#include <cstdlib>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dag_shortest_paths.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, int64_t, boost::property<boost::edge_residual_capacity_t, int64_t, boost::property<boost::edge_reverse_t, traits::edge_descriptor, boost::property<boost::edge_weight_t, int64_t>>>>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

class edge_adder
{
	graph &g;

public:
	explicit edge_adder(graph &g) : g(g) {}
	edge_desc add_edge(int from, int to, int64_t capacity, int64_t cost)
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

		return e;
	}
};

void process_test(void)
{
	uint16_t cities, guides, v_s, v_t;
	uint32_t budget;

	cin >> cities >> guides >> budget >> v_s >> v_t;

	graph g(cities + 2);
	edge_adder adder(g);

	for (uint16_t i = 0; i < guides; i++) {
		uint16_t from, to, cost, capacity;
		cin >> from >> to >> cost >> capacity;
		adder.add_edge(from, to, capacity, cost);
	}

	const uint16_t v_si = cities;
	const uint16_t v_ti = cities + 1;
	const edge_desc e = adder.add_edge(v_si, v_s, numeric_limits<int64_t>::max(), 0);
	adder.add_edge(v_t, v_ti, numeric_limits<int64_t>::max(), 0);

	boost::successive_shortest_path_nonnegative_weights(g, v_si, v_ti);

	int64_t capacity_l = 0;
	int64_t capacity_u = boost::get(boost::edge_capacity, g, e) - boost::get(boost::edge_residual_capacity, g, e);
	int64_t capacity;

	while (capacity_l < capacity_u) {
		capacity = (capacity_u + capacity_l) / (float) 2 + 0.5;
		boost::put(boost::edge_capacity, g, e, capacity);
		boost::successive_shortest_path_nonnegative_weights(g, v_si, v_ti);

		if (boost::find_flow_cost(g) <= budget)
			capacity_l = capacity;
		else
			capacity_u = capacity - 1;
	}

	cout << capacity_l << endl;
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
