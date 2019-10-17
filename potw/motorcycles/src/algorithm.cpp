#include <cstdlib>
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Direction_2 D;

enum biker_status {
	FOREVER,
	STOPPED,
};

typedef struct {
	R path;
	biker_status status;
} Biker;

static const D d_right = D(R(P(0, 0), P(1, 0)));
static const D d_up = D(R(P(0, 0), P(0, 1)));

D mirrored_direction(const D& d)
{
	return D(d.dx(), -d.dy());
}

// TODO Deduplicate code.
void filter_bikers(vector<Biker>& bikers, const vector<pair<int64_t, uint32_t>>& mapping)
{
	Biker* best = &bikers[mapping[0].second];
	D md = mirrored_direction(best->path.direction());
	bool is_q1 = best->path.direction().counterclockwise_in_between(d_right, d_up);

	for (uint32_t i = 1; i < mapping.size(); i++) {
		Biker& b = bikers[mapping[i].second];
		D d = b.path.direction();

		bool len_shorter = (is_q1 and d.counterclockwise_in_between(md, best->path.direction()));
		len_shorter = len_shorter or (!is_q1 and d.counterclockwise_in_between(best->path.direction(), md));
		len_shorter = len_shorter and (best->path.direction() != d_right);
		bool len_equal = d == best->path.direction() || d == md;

		if (CGAL::do_intersect(best->path, b.path)) {
			if (!len_shorter || len_equal)
				b.status = STOPPED;
			if (len_shorter)
				best->status = STOPPED;
		}

		if (len_shorter || (len_equal && d.counterclockwise_in_between(d_right, d_up))) {
			best = &b;
			md = mirrored_direction(best->path.direction());
			is_q1 = best->path.direction().counterclockwise_in_between(d_right, d_up);
		}
	}

	best = &bikers[mapping[mapping.size() - 1].second];
	md = mirrored_direction(best->path.direction());
	is_q1 = best->path.direction().counterclockwise_in_between(d_right, d_up);

	for (int32_t i = mapping.size() - 2; i >= 0; i--) {
		Biker& b = bikers[mapping[i].second];
		D d = b.path.direction();

		bool len_shorter = (is_q1 and d.counterclockwise_in_between(md, best->path.direction()));
		len_shorter = len_shorter or (!is_q1 and d.counterclockwise_in_between(best->path.direction(), md));
		len_shorter = len_shorter and (best->path.direction() != d_right);
		bool len_equal = d == best->path.direction() || d == md;

		if (CGAL::do_intersect(best->path, b.path)) {
			if (!len_shorter && !len_equal)
				b.status = STOPPED;
			if (len_shorter || len_equal)
				best->status = STOPPED;
		}

		if (len_shorter || (len_equal && !d.counterclockwise_in_between(d_right, d_up))) {
			best = &b;
			md = mirrored_direction(best->path.direction());
			is_q1 = best->path.direction().counterclockwise_in_between(d_right, d_up);
		}
	}
}

void process_test(void)
{
	uint32_t n;

	cin >> n;

	vector<Biker> bikers(n);
	vector<pair<int64_t, uint32_t>> mapping(n);

	for (uint32_t i = 0; i < n; i++) {
		int64_t y0, y1;
		uint64_t x1;

		cin >> y0 >> x1 >> y1;

		bikers[i].path = R(P(0, y0), P(x1, y1));
		bikers[i].status = FOREVER;
		mapping[i] = {y0, i};
	}

	struct {
		bool operator()(const pair<int64_t, uint32_t>& a, const pair<int64_t, uint32_t>& b) const {
			return a.first < b.first;
		}
	} sourceLess;

	sort(mapping.begin(), mapping.end(), sourceLess);
	filter_bikers(bikers, mapping);

	for (uint32_t i = 0; i < n; i++) {
		if (bikers[i].status == FOREVER)
			cout << i << " ";
	}

	cout << endl;
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
