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

typedef struct {
	uint16_t reward;
	vector<uint16_t> zones;
} Job;

typedef struct {
	bool buy;
	uint16_t price;
} Zone;

class edge_adder {
	graph &g;

	public:
	explicit edge_adder(graph &g) : g(g) {}

	void add_edge(vertex_desc from, vertex_desc to, int64_t capacity) {
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
	uint16_t Z, J;

	cin >> Z >> J;

	vector<Zone> zones(Z);
	vector<Job> jobs(J);

	for (uint16_t i = 0; i < Z; i++) {
		cin >> zones[i].price;
		zones[i].buy = false;
	}

	for (uint16_t i = 0; i < J; i++)
		cin >> jobs[i].reward;

	for (uint16_t i = 0; i < J; i++) {
		uint16_t neighbors;
		cin >> neighbors;

		for (uint16_t j = 0; j < neighbors; j++) {
			uint16_t zone;
			cin >> zone;
			jobs[i].zones.push_back(zone);
		}
	}

	graph g(0);
	edge_adder adder(g);

	const vertex_desc v_s = J + Z;
	const vertex_desc v_t = v_s + 1;

	for (uint16_t i = 0; i < J; i++) {
		adder.add_edge(v_s, i, jobs[i].reward);

		for (uint16_t j = 0; j < jobs[i].zones.size(); j++)
			adder.add_edge(i, J + jobs[i].zones[j], jobs[i].reward);
	}

	for (uint16_t i = 0; i < Z; i++)
		adder.add_edge(J + i, v_t, zones[i].price);

	int32_t profit;
	bool balanced;
	auto c_map = boost::get(boost::edge_capacity, g);
	const auto rc_map = boost::get(boost::edge_residual_capacity, g);

	do {
		profit = 0;
		balanced = true;
		boost::push_relabel_max_flow(g, v_s, v_t);

		for (int16_t i = 0; i < Z; i++) {
			const edge_desc edge = boost::edge(J + i, v_t, g).first;

			if (rc_map[edge] == 0) {
				profit -= zones[i].price;
				zones[i].buy = true;
			} else {
				zones[i].buy = false;
			}
		}

		for (uint16_t i = 0; i < J; i++) {
			if (jobs[i].reward == 0)
				continue;

			bool accept = true;

			for (const auto& z : jobs[i].zones)
				if (not zones[z].buy)
					accept = false;

			if (accept) {
				profit += jobs[i].reward;
			} else {
				jobs[i].reward = 0;
				const edge_desc edge = boost::edge(v_s, i, g).first;
				c_map[edge] = 0;
				balanced = false;
			}
		}
	} while (not balanced);

	cout << profit << endl;
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
