#include <cstdlib>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, int64_t, boost::property<boost::edge_residual_capacity_t, int64_t, boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

using namespace std;

class edge_adder {
	graph &g;

	public:
	explicit edge_adder(graph &g) : g(g) {}

	void add_edge(uint32_t from, uint32_t to, int64_t capacity, bool force) {
		auto c_map = boost::get(boost::edge_capacity, g);
		auto r_map = boost::get(boost::edge_reverse, g);

		edge_desc e, rev;
		bool e_success, rev_success;

		boost::tie(e, e_success) = boost::edge(from, to, g);
		boost::tie(rev, rev_success) = boost::edge(to, from, g);

		if (!e_success)
			e = boost::add_edge(from, to, g).first;
		if (!rev_success)
			rev = boost::add_edge(to, from, g).first;

		r_map[e] = rev;
		r_map[rev] = e;

		if (force || !e_success)
			c_map[e] = capacity;
		else
			c_map[e] += capacity;

		if (!rev_success)
			c_map[rev] = 0;
	}
};

void process_test(void)
{
	uint16_t w;
	uint32_t n;

	cin >> w >> n;

	graph g(2 * w + 2);
	edge_adder adder(g);

	const vertex_desc v_s = 0;
	const vertex_desc v_t = 2 * w + 1;

	for (uint32_t i = 0; i < n; i++) {
		uint32_t u, v;

		cin >> u >> v;

		if (v < u)
			swap(u, v);

		adder.add_edge(2 * u + 1, 2 * v, 1, false);
		adder.add_edge(2 * v, 2 * v + 1, 1, true);
	}

	adder.add_edge(v_s, v_s + 1, n, true);
	adder.add_edge(v_t - 1, v_t, n, true);

	uint64_t flow = boost::push_relabel_max_flow(g, v_s, v_t);
	cout << flow << endl;
}

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	uint64_t t;

	cin >> t;

	for (uint64_t i = 0; i < t; i++)
		process_test();

	return EXIT_SUCCESS;
}
