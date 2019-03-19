//http://www.cs.princeton.edu/courses/archive/spr10/cos226/assignments/8puzzle.html
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

typedef vector <vector <int>> board;
typedef pair <int, int> coordinate;

struct node {
	board m;
	int dist;
	int steps;
	node(board &x, int y, int z) : m(x), dist(y), steps(z)
	{}
};

class solution {
	public:
		bool astar(board &matrix)
		{
			auto my_greater = [] (const node &a, const node &b) -> bool { return a.dist > b.dist; };
			priority_queue <node, vector <node>, decltype(my_greater)> pq(my_greater);
			unordered_map <string, bool> visited;
			pq.push(node(matrix, 0, 0));

			while(!pq.empty()) {
				node a = pq.top();
				pq.pop();
				if (hamming(a.m) == 0) {
					matrix = a.m;
					return true;
				}
				set_as_visited(a.m, visited);

				vector <board> next;
				get_all_next(a.m, next);
				for (auto &b : next) {
					if (!is_visited(b, visited))
						pq.push(node(b, a.steps + hamming(b), a.steps + 1));
				}
			}

			return false;
		}

	private:
		void get_all_next(board &a, vector <board> &next)
		{
			for (int i = 0; i < a.size(); i++)
				for (int j = 0; j < a[0].size(); j++)
					if (a[i][j] == 0) {
						vector <coordinate> dirs = {
							coordinate(i + 1, j),
							coordinate(i - 1, j),
							coordinate(i, j + 1),
							coordinate(i, j - 1),
						};
						for (auto &k : dirs) {
							if (is_inside(a, k)) {
								board t = a;
								swap(t[i][j], t[k.first][k.second]);
								next.push_back(t);
							}
						}
					}
		}

		bool is_inside(board &a, coordinate &k)
		{
			if (k.first < 0 || k.second < 0 || k.first > a.size() - 1 || k.second > a[0].size() - 1)
				return false;
			return true;
		}

		bool is_visited(board &a, unordered_map <string, bool> &visited)
		{
			string key = get_key_from_board(a);
			return (visited.find(key) != visited.end());
		}

		void set_as_visited(board &a, unordered_map <string, bool> &visited)
		{
			string key = get_key_from_board(a);
			visited[key] =true;
		}

		string get_key_from_board(board &a)
		{
			string ret;
			for (auto &i : a)
				for (auto &j : i)
					ret += (to_string(j) + "|");
			return ret;
		}

		int hamming(board &a)
		{
			int ret = 0;
			int m = a.size();
			int n = a[0].size();
			for (int i = 0; i < m; i++)
				for (int j = 0; j < n; j++)
					if (a[i][j] > 0 && a[i][j] != i * m + j + 1)
						ret += 1;
			return ret;
		}
};

int main(int argc, char **argv)
{
	board matrix = {
		{0, 1, 3},
		{4, 2, 5},
		{7, 8, 6},
	};

	/*not every input is solvable, like this one:
	  board matrix = {
	  {1, 2, 3},
	  {4, 5, 6},
	  {8, 7, 0},
	  };
	 */
	solution s;
	if (s.astar(matrix)) {
		for (auto &i : matrix) {
			for (auto &j : i)
				cout << j << ", ";
			cout << endl;
		}
	}

	exit(EXIT_SUCCESS);
}
