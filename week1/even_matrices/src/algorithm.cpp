#include <cstdlib>
#include <iostream>
#include <vector>

#include <CGAL/Gmpz.h>

using namespace std;
using namespace CGAL;

typedef vector<uint64_t> Row;
typedef vector<Row> Matrix;

Gmpz factorial(uint64_t n)
{
	static map<uint64_t, Gmpz> cache;

	uint64_t start = 1;
	Gmpz result = Gmpz(1);

	for (auto kv : cache) {
		if (kv.first <= n) {
			start = kv.first;
			result = kv.second;
		} else {
			break;
		}
	}

	for (uint64_t i = start + 1; i <= n; i++) {
		result *= i;
	}

	cache.insert(pair<uint64_t, Gmpz>(n, result));

	return result;
}

Gmpz nC2(uint64_t n)
{
	if (n < 2)
		return 0;
	else
		return factorial(n) / 2 / factorial(n - 2);
}

Matrix matrix_sums(const Matrix& m)
{
	uint16_t n;

	n = m.size();
	Matrix sums(n);

	for (uint16_t i = 0; i < n; i++) {
		Row r(n);

		r[0] = m[i][0];

		for (uint16_t j = 1; j < n; j++)
			r[j] = r[j - 1] + m[i][j];

		sums[i] = r;
	}

	for (uint16_t j = 0; j < n; j++) {
		for (uint16_t i = 1; i < n; i++)
			sums[i][j] = sums[i - 1][j] + sums[i][j];
	}

	return sums;
}

Gmpz even_matrices(const Matrix& m)
{
	Gmpz matrices = 0;

	Matrix ms = matrix_sums(m);
	uint16_t n = m.size();

	for (uint16_t end = 0; end < n; end++) {
		for (uint16_t start = 0; start <= end; start++) {
			uint64_t even = 0;
			uint64_t odd = 0;

			for (uint16_t j = 0; j < n; j++) {
				uint64_t subtract = (start > 0) ? ms[start - 1][j] : 0;

				uint64_t result = ms[end][j] - subtract;

				if (result % 2 == 0)
					even++;
				else
					odd++;
			}

			Gmpz pairs;

			pairs = even + nC2(even) + nC2(odd);
			matrices += pairs;
		}
	}

	return matrices;
}

void process_test(void)
{
	uint16_t n;

	cin >> n;
	Matrix m(n);

	for (uint16_t i = 0; i < n; i++) {
		Row r(n);

		for (uint16_t j = 0; j < n; j++) {
			uint16_t next;

			cin >> next;
			r[j] = next;
		}

		m[i] = r;
	}

	Gmpz matrices;

	matrices = even_matrices(m);
	cout << matrices << endl;
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
