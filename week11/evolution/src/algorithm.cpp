#include <cstdlib>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

struct species {
	uint32_t age;
	string name;
	struct species *parent;
	vector<struct species *> children;
	vector<struct species *> ancestors;
};

void process_test(void)
{
	uint32_t n, q;
	cin >> n >> q;

	unordered_map<string, struct species> m;

	for (uint32_t i = 0; i < n; i++) {
		struct species s;
		cin >> s.name >> s.age;
		s.parent = NULL;
		m.insert({s.name, s}).second;
	}

	for (uint32_t i = 0; i < n - 1; i++) {
		string name1, name2;
		cin >> name1 >> name2;

		const auto search1 = m.find(name1);
		const auto search2 = m.find(name2);
		search1->second.parent = &search2->second;
		search2->second.children.push_back(&search1->second);
	}

	struct species *root = NULL;

	for (auto& e : m) {
		if (e.second.parent == NULL) {
			root = &e.second;
			break;
		}
	}

	queue<struct species *> dfs;

	for (auto child : root->children)
		dfs.push(child);

	while (not dfs.empty()) {
		auto current = dfs.front();
		dfs.pop();

		uint32_t level = 0;
		struct species *next = current->parent;
		current->ancestors.push_back(next);

		while (next->ancestors.size() > level) {
			next = next->ancestors[level];
			current->ancestors.push_back(next);
			level++;
		}

		for (auto child : current->children)
			dfs.push(child);
	}

	for (uint32_t i = 0; i < q; i++) {
		string name;
		uint32_t age;
		cin >> name >> age;

		const auto search = m.find(name);
		struct species *current = &search->second;
		int32_t index = current->ancestors.size() - 1;

		while (index >= 0) {
			if (current->ancestors[index]->age <= age) {
				current = current->ancestors[index];
				index = current->ancestors.size() - 1;
			} else {
				index--;
			}
		}

		cout << current->name;

		if (i != q - 1)
			cout << " ";
	}

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
