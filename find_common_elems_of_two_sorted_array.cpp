#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class solution {
public:
	void find_common(vector <int> &large, vector <int> &small, vector <int> &result)
	{
		if (large.size() < small.size()) {
			find_common(small, large, result);
			return;
		}

		find_common(large.begin(), large.end(), small.begin(), small.end(), result);
	}

private:
	template <typename iterator>
	void find_common(iterator l_start, iterator l_end, iterator s_start, iterator s_end, vector <int> &result)
	{
		if (l_start == l_end || s_start == s_end)
			return;

		if (next(s_start) == s_end) {
			if (lower_bound(l_start, l_end, *s_start) != l_end)
				result.push_back(*s_start);
			return;
		}

		iterator l_mid = l_start + distance(l_start, l_end) / 2;
		iterator s_mid = lower_bound(s_start, s_end, *l_mid);
		find_common(l_start, l_mid, s_start, s_mid, result);
		find_common(l_mid, l_end, s_mid, s_end, result);
	}
};


int main(int argc, char **argv)
{
	vector <int> large;
	for (int i = 0; i < 10000; i++)
		large.push_back(i * 2);
	vector <int> small = {-2, -1, 3, 4, 7, 11, 16, 100, 2000, 8000, 30000};

	solution s;
	vector <int> result;
	s.find_common(large, small, result);
	for (auto &i : result)
		cout << i << ", ";
	cout << endl;
	exit(EXIT_SUCCESS);
}
