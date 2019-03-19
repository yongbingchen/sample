#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

typedef unordered_map <int, unordered_map <int, int>> matrix;

class solution {
	public:
		void multiply_sparse_matrix(matrix &m1, matrix &m2, matrix &result) //can not claim the map as const, because operator [] is not.
		{
			unordered_map <int, vector <int>> rows; //use row number as key, store all non-zero elem's col at 'val' vector.
			unordered_map <int, vector <int>> cols; //use col number as key, store all non-zero elem's row at 'val' vector.

			get_all_non_zero_rows(m1, rows);
			get_all_non_zero_cols(m2, cols);

			for (auto &i : rows) {
				for (auto &j : cols) {
					int ret = 0;
					for (auto &k : i.second) {
						if (find(j.second.begin(), j.second.end(), k) != j.second.end())
							ret += m1[i.first][k] * m2[k][j.first]; //write down a real dot multiply to figure out this equation.
					}
					if (ret != 0)
						result[i.first][j.first] = ret;
				}
			}
		}

	private:
		void get_all_non_zero_rows(const matrix &m, unordered_map <int, vector <int>> &rows)
		{
			for (auto &i : m)
				for (auto &j : i.second)
					rows[i.first].push_back(j.first);
		}

		void get_all_non_zero_cols(const matrix &m, unordered_map <int, vector <int>> &cols)
		{
			for (auto &i : m)
				for (auto &j : i.second)
					cols[j.first].push_back(i.first);
		}


};

int main(int argc, char **argv)
{
	matrix sparse_m1;
	sparse_m1[1][2] = 10;
	sparse_m1[1][4] = 12;
	sparse_m1[3][3] = 5;
	sparse_m1[4][1] = 15;
	sparse_m1[4][2] = 12;

	matrix sparse_m2;
	sparse_m2[1][3] = 8;
	sparse_m2[2][4] = 23;
	sparse_m2[3][3] = 9;
	sparse_m2[4][1] = 20;
	sparse_m2[4][2] = 25;


	solution s;
	matrix result;
	cout << "multiply sparse matrix" << endl;
	s.multiply_sparse_matrix(sparse_m1, sparse_m2, result);
	for (auto &i : result)
		for (auto &j : i.second)
			cout << i.first << ", " << j. first << ", val " << result[i.first][j.first] << endl;

	exit(EXIT_SUCCESS);
}
