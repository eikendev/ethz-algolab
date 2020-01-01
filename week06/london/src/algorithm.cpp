#include <cstdlib>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, int64_t, boost::property<boost::edge_residual_capacity_t, int64_t, boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
	graph &g;

	public:
	explicit edge_adder(graph &g) : g(g) {}

	void add_edge(vertex_desc from, vertex_desc to, int64_t capacity, bool reset) {
		auto c_map = boost::get(boost::edge_capacity, g);
		auto r_map = boost::get(boost::edge_reverse, g);

		edge_desc e, rev;
		bool e_success, rev_success;

		boost::tie(e, e_success) = boost::edge(from, to, g);
		boost::tie(rev, rev_success) = boost::edge(to, from, g);

		if (not e_success)
			e = boost::add_edge(from, to, g).first;

		if (not rev_success)
			rev = boost::add_edge(to, from, g).first;

		r_map[e] = rev;
		r_map[rev] = e;

		if (reset or not e_success)
			c_map[e] = capacity;
		else
			c_map[e] += capacity;

		if (not rev_success)
			c_map[rev] = 0;
	}
};

void process_test(void)
{
	uint16_t h, w;
	string chars;

	cin >> h >> w >> chars;

	const uint16_t possible_chars = 'Z' - 'A' + 1;
	const uint64_t page_offset = possible_chars * possible_chars;

	graph g(page_offset + possible_chars + 2);
	edge_adder adder(g);

	const vertex_desc v_s = page_offset + possible_chars;
	const vertex_desc v_t = v_s + 1;

	// Add a flow of 1 from each piece to the target.
	for (uint64_t i = 0; i < chars.size(); i++)
		adder.add_edge(page_offset + (chars[i] - 'A'), v_t, 1, false);

	vector<string> page1(h);

	for (uint16_t i = 0; i < h; i++)
		cin >> page1[i];

	vector<string> page2(h);

	for (uint16_t i = 0; i < h; i++)
		cin >> page2[i];

	vector<vector<int64_t>> capacities(possible_chars, vector<int64_t>(possible_chars, 0));

	for (uint16_t i = 0; i < h; i++) {
		for (uint16_t j = 0; j < w; j++) {
			const uint16_t column = w - 1 - j;

			if (page1[i][j] > page2[i][column])
				swap(page1[i][j], page2[i][column]);

			capacities[page1[i][j] - 'A'][page2[i][column] - 'A']++;
		}
	}

	for (uint16_t i = 0; i < possible_chars; i++) {
		for (uint16_t j = i; j < possible_chars; j++) {
			const uint64_t combination = i * possible_chars + j;

			// Add a flow of 1 from the source to the current combination.
			adder.add_edge(v_s, combination, capacities[i][j], false);

			// Add a flow of 1 from the current combination to the respective pieces.
			adder.add_edge(combination, page_offset + i, capacities[i][j], false);
			adder.add_edge(combination, page_offset + j, capacities[i][j], false);
		}
	}

	// Check if the maximum flow equals the size of chars.
	int64_t flow = boost::push_relabel_max_flow(g, v_s, v_t);

	if (chars.size() == flow)
		cout << "Yes" << endl;
	else
		cout << "No" << endl;
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
