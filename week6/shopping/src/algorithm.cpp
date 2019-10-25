#include <cstdlib>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, uint64_t, boost::property<boost::edge_residual_capacity_t, uint64_t, boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

using namespace std;

class edge_adder {
	graph &g;

 	public:
	explicit edge_adder(graph &g) : g(g) {}

	void add_edge(uint64_t from, uint64_t to, int64_t capacity) {
		auto c_map = boost::get(boost::edge_capacity, g);
		auto r_map = boost::get(boost::edge_reverse, g);

		edge_desc e, rev;
		bool e_success, rev_success;

		boost::tie(e, e_success) = boost::add_edge(from, to, g);
		boost::tie(rev, rev_success) = boost::add_edge(to, from, g);

		r_map[e] = rev;
		r_map[rev] = e;

		if (e_success)
			c_map[e] = capacity;
		else
			c_map[e] += capacity;

		if (rev_success)
			c_map[rev] = 0;
	}
};

void process_test(void)
{
	uint64_t n, m, s;

	cin >> n >> m >> s;

	graph g(n + 2);
	edge_adder adder(g);

	const uint64_t v_s = n;
	const uint64_t v_t = n + 1;

	adder.add_edge(v_s, 0, s);

	for (uint64_t i = 0; i < s; i++) {
		uint64_t u;

		cin >> u;
		adder.add_edge(u, v_t, 1);
	}

	for (uint64_t i = 0; i < m; i++) {
		uint64_t u, v;

		cin >> u >> v;
		adder.add_edge(u, v, 1);
		adder.add_edge(v, u, 1);
	}

	uint64_t flow = boost::push_relabel_max_flow(g, v_s, v_t);

	if (flow == s)
		cout << "yes" << endl;
	else
		cout << "no" << endl;
}

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	uint64_t t;

	cin >> t;

	for (uint32_t i = 0; i < t; i++)
		process_test();

	return EXIT_SUCCESS;
}
