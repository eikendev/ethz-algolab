#include <cstdlib>
#include <iostream>
#include <numeric>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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

	void add_edge(const vertex_desc from, const vertex_desc to, const uint64_t capacity)
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

vector<vertex_desc> cut2vector(graph& g, const vertex_desc from, const vertex_desc to)
{
	boost::push_relabel_max_flow(g, from, to);

	const uint64_t n = num_vertices(g);
	vector<bool> visited(n, false);
	queue<vertex_desc> bfs;
	visited[from] = true;
	bfs.push(from);

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

	vector<vertex_desc> figures;
	figures.reserve(n);

	for (uint64_t i = 0; i < n; i++)
		if (visited[i])
			figures.push_back(i);

	return figures;
}

void process_test(void)
{
	uint16_t n, m;
	cin >> n >> m;

	graph g(n);
	edge_adder adder(g);

	for (uint16_t i = 0; i < m; i++) {
		uint16_t a, b, c;
		cin >> a >> b >> c;
		adder.add_edge(a, b, c);
	}

	uint64_t best_cost = numeric_limits<uint64_t>::max();
	uint16_t best_from = 0;
	uint16_t best_to = 1;
	const uint16_t v_s = 0;

	for (uint16_t v_t = 1; v_t < n; v_t++) {
		const auto update_cost = [&](const vertex_desc from, const vertex_desc to) {
			const uint64_t cost = boost::push_relabel_max_flow(g, from, to);

			if (cost <= best_cost) {
				best_cost = cost;
				best_from = from;
				best_to = to;
			}
		};

		update_cost(v_s, v_t);
		update_cost(v_t, v_s);
	}

	vector<vertex_desc> figures = cut2vector(g, best_from, best_to);

	cout << best_cost << endl;
	cout << figures.size() << " ";

	for (const auto& f : figures)
		cout << f << " ";

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
