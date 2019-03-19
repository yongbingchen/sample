/*
Prroblem: write an algorithm to calculate the minimum cost to add new roads between the cities such that all the cities are accessible from each other

int numTotalAvailableCities = 6;
int numTotalAvailableRoads = 3;
int[,] roadsAvailable = { { 1, 4 }, { 4, 5 }, { 2, 3 } };
int[,] costNewRoadsToConstruct = { { 1, 2,5 }, { 1,3,10 }, {1,6,2} ,{ 5, 6, 5 } };
int numNewRoadsConstruct = 4;
*/

/*
https://en.wikipedia.org/wiki/Minimum_spanning_tree
my solution:
1. build a graph from existing roads.
2. use bfs to divide vertices into islands.
	if islands.size() == 1, stop
3. build another graph from new roads of candidate.
4. for each pair of islands of #2, try to connect the closest pair (through the candidates of #3):
	4.1 for each vertex of island $A (vertex $a), start a Dijkstra algorithm in graph #3, find the minimum of the cost to all other vertex.
		4.1.1 if the resulted vertices (can be reached from vertex $a) have overlap with island $B, then find the minimum cost path from $a to $B.
		4.1.2 the min of all $a to $B is the distance from island $A to $B.
	4.3 iterate over all pair of islands, find the closest pair of islands (along with the path from $A to $B).
5. remove all edges of the path of #4.3 from new roads set, add them to existed roads set, goto #1.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

struct edge {
	int src;
	int dst;
	int weight;
	edge(int x, int y, int z) : src(x), dst(y), weight(z) {}
	bool operator == (const edge &a) const
	{
		return a.src == this->src && a.dst == this->dst && a.weight == this->weight ||
			a.src == this->dst && a.dst == this->src && a.weight == this->weight;
	}
};

struct node {
	int vertex;
	int pred;
	int cost;
	node(int x, int y, int z) : vertex(x), pred(y), cost(z) {}
};

class graph {
public:
	graph(vector <edge> &edges, int v)
	{
		vertices = v;
		adja.resize(v);
		for (auto &e : edges) {
			adja[e.src].push_back(e);
			adja[e.dst].push_back(e);
		}
	}

	void dijkstra(const int start, vector <node> &result)
	{
		vector <bool> done(vertices, false);
		auto my_greater = [] (const node &a, const node &b)
			{ return a.cost > b.cost; };
		priority_queue<node, vector <node>, decltype(my_greater)> pq(my_greater);

		vector <int> cost(vertices, INT_MAX);
		cost[start] = 0;
		pq.push(node(start, -1, 0));

		while(!pq.empty()) {
			node u = pq.top();
			pq.pop();
			done[u.vertex] = true;
			result.push_back(u);

			for (auto &v : adja[u.vertex]) {
				int neighbor = v.src == u.vertex ? v.dst : v.src;
				if (!done[neighbor]) {
					if (cost[u.vertex] + v.weight < cost[neighbor]) {
						cost[neighbor] = cost[u.vertex] + v.weight;
						pq.push(node(neighbor, u.vertex, cost[neighbor]));
					}
				}
			}
		}
	}

	void get_connectivity(const int start, vector <int> &connected, vector <bool> &visited)
	{
		queue <int> q;
		q.push(start);
		while(!q.empty()) {
			int u = q.front();
			q.pop();
			visited[u] = true;
			connected.push_back(u);

			for (auto &v : adja[u]) {
				int neighbor = v.src == u ? v.dst : v.src;
				if (!visited[neighbor])
					q.push(neighbor);
			}
		}
	}
private:
	int vertices;
	vector <vector <edge>> adja;
};

class solution {
public:
	void get_minimum_cost(vector <edge> &existing,
			vector <edge> &planning,
			const int vertices)
	{
		graph exist(existing, vertices);
		graph plan(planning, vertices);

		vector <vector <int>> islands;
		vector <bool> visited(vertices, false);
		for (int i = 0; i < vertices; i++) {
			vector <int> island;
			if (!visited[i]) {
				exist.get_connectivity(i, island, visited);
			}
			if (island.size() != 0)
				islands.push_back(island);
		}

		if (islands.size() == 1) //all cities are connected now
			return;

		vector <edge> new_roads;
		find_closest_pair_of_island(islands, plan, new_roads);
		connect_islands_and_update_edges(existing, planning, new_roads);
		//recursive call, since we updated the graph.
		get_minimum_cost(existing, planning, vertices);
	}

private:
	void find_closest_pair_of_island(const vector <vector <int>> &islands,
					graph &plan,
					vector <edge> &new_roads)
	{
		int min_dist = INT_MAX;
		vector <edge> curr;
		for (int i = 0; i < islands.size() - 1; i++) {
			for (int j = i + 1; j < islands.size(); j++) {
				int ret = find_distance_of_two_islands(plan,
						islands[i], islands[j], curr);
				if (min_dist > ret) {
					min_dist = ret;
					new_roads = curr;
				}
			}
		}
	}

	int find_distance_of_two_islands(graph &plan,
			const vector <int> &i_a,
			const vector <int> &i_b,
			vector <edge> &curr)
	{
		int distance = INT_MAX;
		for (auto &i : i_a) {
			vector <node> result;
			plan.dijkstra(i, result);
			if (result.size() != 0) {
				vector <edge> edges;
				int ret = find_distance_from_vertex_to_island(i,
						result, i_b, edges);
				if (distance > ret) {
					distance = ret;
					curr = edges;
				}
			}
		}
		return distance;
	}

	//find closest node in island $B to vertex $a in island $A
	int find_distance_from_vertex_to_island(const int start,
			const vector <node> &result,
			const vector <int> &island,
			vector <edge> &edges)
	{
		int distance = INT_MAX;
		const node *closest = nullptr;
		for (auto &i : result) {
			auto it = find(island.begin(), island.end(), i.vertex);
			if (it != island.end()) {
				if (distance > i.cost) {
					distance = i.cost;
					closest = &i;
				}
			}
		}

		if (closest == nullptr) //can not reach island $B from vertex $start
			return INT_MAX;

		int pred = closest->pred;
		int curr = closest->vertex;
		if (pred == start)
			edges.push_back(edge(curr, pred, closest->cost));

		while(pred != start) {
			auto my_compare_1 = [pred] (const node &a) { return a.vertex == pred; };
			auto pre = find_if(result.begin(), result.end(), my_compare_1);
			auto my_compare_2 = [curr] (const node &a) { return a.vertex == curr; };
			auto cur = find_if(result.begin(), result.end(), my_compare_2);
			int weight = cur->cost - pre->cost;
			edges.push_back(edge(curr, pred, weight));
			curr = pre->vertex;
			pred = pre->pred;
		}

		return distance;
	}

	void connect_islands_and_update_edges(vector <edge> &existing,
						vector <edge> &planning,
						const vector <edge> &new_roads)
	{
		for (auto &e : new_roads) {
			auto my_compare = [e] (const edge &a) { return a == e; };
			auto it = find_if(planning.begin(), planning.end(), my_compare);
			planning.erase(it);
			existing.push_back(e);
		}
	}
};

int main(int argc, char **argv)
{
#ifdef CASE_FROM_THIS_TOPIC
	/*
	 * int[,] roadsAvailable = { { 1, 4 }, { 4, 5 }, { 2, 3 } };
	 int[,] costNewRoadsToConstruct = { { 1, 2,5 }, { 1,3,10 }, {1,6,2} ,{ 5, 6, 5 } };
	 */
	vector <edge> existing = {
		edge(0, 3, 0),
		edge(3, 4, 0),
		edge(1, 2, 0),
	};

	vector <edge> planning = {
		edge(0, 1, 5),
		edge(0, 2, 10),
		edge(0, 5, 2),
		edge(4, 5, 5),
	};

	int vertices = 6;
#else //https://www.statisticshowto.datasciencecentral.com/minimum-spanning-tree
	vector <edge> existing = {
		edge(0, 2, 0),
		edge(2, 3, 0),
		edge(2, 4, 0),
	};

	vector <edge> planning = {
		edge(0, 1, 5),
		edge(1, 3, 8),
		edge(1, 5, 6),
		edge(3, 5, 8),
		edge(4, 5, 7),
		edge(4, 6, 9),
	};

	int vertices = 7;
#endif
	solution s;
	s.get_minimum_cost(existing, planning, vertices);
	cout << "final roads connect all cities:" << endl;
	for (auto &i : existing) {
		cout << "road from " << i.src << " to " << i.dst << endl;
	}
}
