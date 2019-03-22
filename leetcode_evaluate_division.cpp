/*
Equations are given in the format A / B = k, where A and B are variables represented as strings, and k is a real number (floating point number). Given some queries, return the answers. If the answer does not exist, return -1.0.

Example:
Given a / b = 2.0, b / c = 3.0.
queries are: a / c = ?, b / a = ?, a / e = ?, a / a = ?, x / x = ? .
return [6.0, 0.5, -1.0, 1.0, -1.0 ].

The input is: vector<pair<string, string>> equations, vector<double>& values, vector<pair<string, string>> queries , where equations.size() == values.size(), and the values are positive. This represents the equations. Return vector<double>.

According to the example above:

equations = [ ["a", "b"], ["b", "c"]. ["c", "d"] ],
values = [2.0, 3.0, 4.0],
queries = [ ["a", "d"], ["b", "a"], ["a", "e"], ["a", "a"], ["x", "x"] ].
The input is always valid. You may assume that evaluating the queries will result in no division by zero and there is no contradiction.
*/

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

struct edge {
	int src;
	int dst;
	double weight;
	edge(int x, int y, double z) : src(x), dst(y), weight(z) {}
};

class graph {
public:
	graph(const vector <edge> edges, const int v)
	{
		vertices = v;
		adja.resize(v);
		for (auto &e : edges) {
			adja[e.src].push_back(e);
			adja[e.dst].push_back(edge(e.dst, e.src, 1 / e.weight));
		}
	}

	bool travel(const int start, const int end, double &val)
	{
		val = 1;
		vector <bool> visited(vertices, false);
		return dfs(start, end, val, visited);
	}
private:
	bool dfs(const int start, const int end, double &val, vector <bool> &visited)
	{
		if (start == end)
			return true;

		visited[start] = true;
		for (auto &e : adja[start]) {
			if (!visited[e.dst]) {
				double tmp = val;
				val *= e.weight;
				if (dfs(e.dst, end, val, visited))
					return true;
				val = tmp; //backtracking
			}
		}
		return false;
	}
	int vertices;
	vector <vector <edge>> adja;
};

class Solution {
public:
	vector<double> calcEquation(const vector<pair<string, string>> eq, const vector<double>& values, const vector<pair<string, string>> qu)
	{
		//get all variables
		set <string> var;
		for (auto &i : eq) {
			var.insert(i.first);
			var.insert(i.second);
		}
		int vertices = var.size();

		//use the equations to build edge between variables
		vector <edge> edges;
		for (int i = 0; i < eq.size(); i++) {
			auto it = lower_bound(var.begin(), var.end(), eq[i].first);
			int idx_src = distance(var.begin(), it);
			it = lower_bound(var.begin(), var.end(), eq[i].second);
			int idx_dst = distance(var.begin(), it);
			edges.push_back(edge(idx_src, idx_dst, values[i]));
		}
		graph g(edges, vertices);

		vector <double> ret;
		for (auto &i : qu) {
			auto it = lower_bound(var.begin(), var.end(), i.first);
			if (it == var.end() || *it != i.first) {
				ret.push_back(-1);
				continue;
			}
			int start = distance(var.begin(), it);
			it = lower_bound(var.begin(), var.end(), i.second);
			if (it == var.end() || *it != i.second) {
				ret.push_back(-1);
				continue;
			}
			int end = distance(var.begin(), it);

			double val;
			if (g.travel(start, end, val)) {
				ret.push_back(val);
			}
			else {
				//failed to find a path from dividend to divisor
				ret.push_back(-1);
			}
		}
		return ret;
	}
};

int main(int argc, char **argv)
{
	Solution s;
	vector <pair <string, string>> eq = {
		pair <string, string>("a", "b"),
		pair <string, string>("b", "c"),
		pair <string, string>("d", "b"),
	};
	vector <double> val = {2.0, 3.0, 4.0};
	vector <pair <string, string>> qu = {
		pair <string, string>("a", "c"),
		pair <string, string>("a", "d"),
		pair <string, string>("b", "a"),
		pair <string, string>("a", "e"),
		pair <string, string>("a", "a"),
		pair <string, string>("x", "x"),
		pair <string, string>("a", "d"),
	};

	vector <double> ret;
	ret = s.calcEquation(eq, val, qu);
	for (auto &i : ret)
		cout << i << ", ";
	cout << endl;
}
