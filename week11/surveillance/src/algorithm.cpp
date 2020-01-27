#include <cstdlib>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

typedef boost::default_color_type color;
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, uint64_t, boost::property<boost::edge_residual_capacity_t, uint64_t, boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;

const color black = boost::color_traits<color>::black();

class edge_adder
{
	graph &g;

public:
	explicit edge_adder(graph &g) : g(g) {}

	void add_edge(const vertex_desc from, const vertex_desc to, uint64_t capacity)
	{
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
	uint16_t n, m, k, l;
	cin >> n >> m >> k >> l;

	vector<vertex_desc> stations(k);
	vector<uint16_t> stations_count(n, 0);

	for (uint16_t i = 0; i < k; i++) {
		cin >> stations[i];
		stations_count[stations[i]]++;
	}

	vector<vertex_desc> photographs(l);
	vector<uint16_t> photograph_count(n, 0);

	for (uint16_t i = 0; i < l; i++) {
		cin >> photographs[i];
		photograph_count[photographs[i]]++;
	}

	graph g(n);

	for (uint16_t i = 0; i < m; i++) {
		uint16_t from, to;
		cin >> from >> to;
		boost::add_edge(from, to, g);
	}

	graph network(n);
	edge_adder adder(network);

	for (const auto& e : boost::make_iterator_range(boost::edges(g)))
		adder.add_edge(boost::source(e, g), boost::target(e, g), 1);

	vector<vertex_desc> photograph_limit(n);

	for (const auto& p : boost::make_iterator_range(boost::vertices(g))) {
		if (photograph_count[p] == 0)
			continue;

		photograph_limit[p] = boost::add_vertex(network);
		adder.add_edge(photograph_limit[p], p, photograph_count[p]);
	}

	const vertex_desc v_source = boost::add_vertex(network);
	const vertex_desc v_target = boost::add_vertex(network);

	for (const auto& s : boost::make_iterator_range(boost::vertices(g))) {
		if (stations_count[s] == 0)
			continue;

		adder.add_edge(s, v_target, stations_count[s]);
		const vertex_desc station = boost::add_vertex(network);
		adder.add_edge(v_source, station, stations_count[s]);

		vector<color> vertex_color(n);
		boost::breadth_first_search(g, s, boost::color_map(boost::make_iterator_property_map(vertex_color.begin(), boost::get(boost::vertex_index, g))));

		for (const auto& p : boost::make_iterator_range(boost::vertices(g)))
			if (vertex_color[p] == black and photograph_count[p] > 0)
				adder.add_edge(station, photograph_limit[p], numeric_limits<uint64_t>::max());
	}

	uint64_t flow = boost::push_relabel_max_flow(network, v_source, v_target);

	cout << flow << endl;
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
