#include <iostream>
#include <vector>

using namespace std;

struct node {
	string val;
	vector <node *>next;
	node(string x) : val(x), next(vector <node *>(26, nullptr))
	{}
};

class trie {
	public:
		void build_tree(node *root, string word)
		{
			node *p = root;
			for (auto &c : word) {
				if (p->next[c - 'a'] == nullptr)
					p->next[c - 'a'] = new node("");
				p = p->next[c - 'a'];
			}
			p->val = word;
		}

		void search(node *root, string pattern, int idx, vector <string> &result)
		{
			if (root == nullptr)
				return;

			if (idx == pattern.size()) {
				if (root->val.size() != 0)
					result.push_back(root->val);
				return;
			}

			if (pattern[idx] != '*') {
				return search(root->next[pattern[idx] - 'a'], pattern, idx + 1, result);

			}

			 /*case 1: let '*' match zero char*/
			search(root, pattern, idx + 1, result);
			/*case 2: let '*' match curr char, and keep matching on its child*/
			for (auto &i : root->next) {
				search(i, pattern, idx, result);
			}
		}
};

int main(int argc, char **argv)
{
	vector <string> dict = {
		"lexicographic", "sorting", "of", "a", "set", "of", "keys", "can",
		"be", "accomplished", "with", "a", "simple", "trie", "based",
		"algorithm", "we", "insert", "all", "keys", "in", "a", "trie",
		"output", "all", "keys", "in", "the", "trie", "by", "means", "of",
		"preorder", "traversal", "which", "results", "in", "output", "that",
		"is", "in", "lexicographically", "increasing", "order", "preorder",
		"traversal", "is", "a", "kind", "of", "depth", "first", "traversal"
	};

	trie t;
	node *root = new node("");
	for (auto &i : dict)
		t.build_tree(root, i);
	vector <string> patterns = {"o*put", "lexi*", "lexi*ph*", "sort*", "first", "*traversal", "*versal", "*preoder", "pre*ode", "preorder*", "preoder*l"};
	for (auto &i : patterns) {
		vector <string> result;
		t.search(root, i, 0, result);
		cout << "search pattern " << i << ", get result: ";
		for (auto &s : result)
			cout << s << ",";
		cout << endl;
	}
	exit(EXIT_SUCCESS);
}
