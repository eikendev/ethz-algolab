#include <cassert>
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

	void add_edge(uint32_t from, uint32_t to, int64_t capacity) {
		auto c_map = boost::get(boost::edge_capacity, g);
		auto r_map = boost::get(boost::edge_reverse, g);
		const auto e = boost::add_edge(from, to, g).first;
		const auto rev_e = boost::add_edge(to, from, g).first;
		c_map[e] = capacity;
		c_map[rev_e] = 0;
		r_map[e] = rev_e;
		r_map[rev_e] = e;
	}
};

void process_test(void)
{
	uint16_t n, m;

	cin >> n >> m;

	const uint16_t v_s = n + m + 0;
	const uint16_t v_t = n + m + 1;

	graph g(n + m + 2);
	edge_adder adder(g);

	for (uint16_t i = n; i < n + m; i++) {
		uint16_t a, b, c;

		cin >> a >> b >> c;
		adder.add_edge(v_s, i, 1);

		switch (c) {
		case 0:
			adder.add_edge(i, a, 1);
			adder.add_edge(i, b, 1);
			break;
		case 1:
			adder.add_edge(i, a, 1);
			break;
		case 2:
			adder.add_edge(i, b, 1);
			break;
		default:
			assert(false);
		}
	}

	uint32_t total = 0;

	for (uint16_t i = 0; i < n; i++) {
		uint16_t s;

		cin >> s;
		total += s;

		if (s > 0)
			adder.add_edge(i, v_t, s);
	}

	int64_t flow = boost::push_relabel_max_flow(g, v_s, v_t);

	if (flow == total && flow == m)
		cout << "yes" << endl;
	else
		cout << "no" << endl;
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
