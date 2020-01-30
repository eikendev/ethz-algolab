#include <cstdlib>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, uint64_t, boost::property<boost::edge_residual_capacity_t, uint64_t, boost::property<boost::edge_reverse_t, traits::edge_descriptor, boost::property<boost::edge_weight_t, int64_t>>>>> graph;
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
		const auto e = boost::add_edge(from, to, g).first;
		const auto rev_e = boost::add_edge(to, from, g).first;
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
	uint16_t n, s;
	cin >> n >> s;

	graph g;
	edge_adder adder(g);

	const auto v_source = boost::add_vertex(g);
	const auto v_target = boost::add_vertex(g);

	vector<uint16_t> initial(s);
	uint16_t cars = 0;

	for (uint16_t i = 0; i < s; i++) {
		cin >> initial[i];
		cars += initial[i];
	}

	const uint16_t p_max = 100;
	const uint64_t time_max = 100000;

	vector<vector<pair<bool, vertex_desc>>> vmap(s, vector<pair<bool, vertex_desc>>(time_max + 1, make_pair(false, 0)));

	for (uint16_t i = 0; i < n; i++) {
		uint16_t from, to, p;
		uint32_t d, a;
		cin >> from >> to >> d >> a >> p;

		from--;
		to--;

		if (not vmap[from][d].first)
			vmap[from][d].second = boost::add_vertex(g);

		if (not vmap[to][a].first)
			vmap[to][a].second = boost::add_vertex(g);

		vmap[from][d].first = true;
		vmap[to][a].first = true;

		const uint64_t cost = (a - d) * p_max - p;
		adder.add_edge(vmap[from][d].second, vmap[to][a].second, 1, cost);
	}

	for (uint16_t i = 0; i < s; i++) {
		auto from = v_source;
		uint32_t from_time = 0;

		for (uint32_t j = 0; j <= time_max; j++) {
			const auto v = vmap[i][j];

			if (not v.first)
				continue;

			const uint64_t capacity = (from == v_source) ? initial[i] : numeric_limits<uint64_t>::max();
			const int64_t cost = (j - from_time) * p_max;
			adder.add_edge(from, v.second, capacity, cost);

			from = v.second;
			from_time = j;
		}

		const uint64_t capacity = (from == v_source) ? initial[i] : numeric_limits<uint64_t>::max();
		const int64_t cost = (time_max - from_time) * p_max;
		adder.add_edge(from, v_target, capacity, cost);
	}

	boost::successive_shortest_path_nonnegative_weights(g, v_source, v_target);
	int64_t cost = boost::find_flow_cost(g);
	cost = time_max * cars * p_max - cost;

	cout << cost << endl;
}

int main(void)
{
	ios_base::sync_with_stdio(false);

	uint32_t t;

	cin >> t;

	for (uint32_t i = 0; i < t; i++)
		process_test();

	return EXIT_SUCCESS;
}
