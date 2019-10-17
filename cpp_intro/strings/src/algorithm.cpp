#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

string reverse_swap(string s1, string s2)
{
	std::reverse(s1.begin(), s1.end());
	std::reverse(s2.begin(), s2.end());

	char c = s1[0];
	s1[0] = s2[0];
	s2[0] = c;

	return s1 + " " + s2;
}

void process_line(void)
{
	string s1, s2;

	cin >> s1 >> s2;

	cout << s1.length() << " " << s2.length() << endl;
	cout << s1 << s2 << endl;
	cout << reverse_swap(s1, s2) << endl;
}

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	uint16_t t;

	cin >> t;

	for (uint8_t i = 0; i < t; i++)
		process_line();

	return EXIT_SUCCESS;
}
