#include <algorithm>
#include <cstdlib>
#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<uint32_t, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

void process_test(void)
{
	uint32_t m, n;
	cin >> m >> n;

	vector<pair<K::Point_2, uint64_t>> participants;
	participants.reserve(m);

	for (uint32_t i = 0; i < m; i++) {
		int64_t x, y;
		uint64_t r;
		cin >> x >> y >> r;
		participants.emplace_back(K::Point_2(x, y), r);
	}

	uint64_t h;
	cin >> h;

	vector<pair<K::Point_2, uint32_t>> lamps;
	lamps.reserve(n);

	for (uint32_t i = 0; i < n; i++) {
		int64_t x, y;
		cin >> x >> y;
		lamps.emplace_back(K::Point_2(x, y), i);
	}

	Triangulation t;
	t.insert(lamps.begin(), lamps.end());

	vector<uint32_t> revealed_by(m, n);
	uint32_t winning_lamp = 0;

	for (uint32_t i = 0; i < m; i++) {
		const auto center = participants[i].first;
		const auto radius = participants[i].second;
		uint32_t revealer = n;

		const auto nearest = t.nearest_vertex(center);
		const auto lamp = nearest->point();
		const auto limit = K::FT(h * h + radius * radius + 2 * h * radius);

		if (CGAL::squared_distance(lamp, center) < limit) {
			revealer = min(revealer, (uint32_t) nearest->info());

			for (uint32_t j = 0; j < revealer; j++) {
				if (CGAL::squared_distance(lamps[j].first, center) < limit) {
					revealer = min(revealer, j);
					break;
				}
			}
		}

		revealed_by[i] = revealer;
		winning_lamp = max(winning_lamp, revealer);
	}

	for (uint32_t i = 0; i < m; i++)
		if (revealed_by[i] == winning_lamp)
			cout << i << " ";

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
