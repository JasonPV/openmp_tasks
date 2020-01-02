#include <iostream>
#include <chrono>
#include <omp.h>
#include <ctime>

using namespace std;

int** mult_block(int** arr1, int** arr2, int n, int m, int k, int threads_num);
int** mult_lent(int** arr1, int** arr2, int n, int m, int k, int threads_num);


int main()
{
	for (int threads_number = 1; threads_number < 17; threads_number *= 2)
	{
		for (int dim = 10; dim <= 1000; dim *= 10)
		{
			srand(time(0));
			int n = dim;
			int m = dim;
			int k = dim;
			int** arr1 = new int* [n];
			int** arr2 = new int*[m];
			for (int i = 0; i < n; i++)
				arr1[i] = new int[m];
			for (int i = 0; i < m; i++)
				arr2[i] = new int[k];

			for (int i = 0; i < n; i++)
				for (int j = 0; j < m; j++)
				{
					arr1[i][j] = rand() % 100;
					arr2[i][j] = rand() % 100;
				}


			auto start1 = chrono::system_clock::now();
			int** result1 = mult_block(arr1, arr2, n, m, k, threads_number);
			auto end1 = chrono::system_clock::now();

			auto start2 = chrono::system_clock::now();
			int** result2 = mult_lent(arr1, arr2, n, m, k, threads_number);
			auto end2 = chrono::system_clock::now();


			cout << "for block:" << endl << "dim = " << dim << endl << "threads_number = " << threads_number << endl << "time = ";
			cout << chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count() << endl;

			cout << "for tapes:" << endl << "dim = " << dim << endl << "threads_number = " << threads_number << endl << "time = ";
			cout << chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count() << endl;
		}

	}

	return 0;
}

	int** mult_block(int** arr1, int** arr2, int n, int m, int k, int threads_num)
	{
		int** result = new int* [m];
		for (int i = 0; i < m; i++)
		result[i] = new int[k];
		for (int i = 0; i < m; i++)
		for (int j = 0; j < k; j++)
			result[i][j] = 0;
#pragma omp parallel num_threads(threads_num)
	{
		int p = omp_get_num_threads();
		int q = p;
		int r = p;
		int a_h = m / p;
		int a_w = n / q;
		int b_w = k / r;
#pragma omp for
		for (int a_block_count = 0; a_block_count < p * q; a_block_count++) {
			int i_block = a_block_count / q;
			int j_block = a_block_count % q;
			for (int t_block = 0; t_block < r; t_block++) {
				for (int i = i_block * a_h; i < (i_block + 1) * a_h; i++)
					for (int j = j_block * a_w; j < (j_block + 1) * a_w; j++)
						for (int t = t_block * b_w; t < (t_block + 1) * b_w; t++)
							result[i][t] += arr1[i][j] * arr2[j][t];
			}
		}
	}
		return result;

	}

	int** mult_lent(int** arr1, int** arr2, int n, int m, int k, int threads_num)
	{

			int i, j, s, sum;
			int** result = new int*[n];
			for (int i = 0; i < n; i++)
				result[i] = new int[m];

#pragma omp parallel for private(j,s,sum) num_threads(threads_num)
			for (i = 0; i < n; i++) {
				for (s = 0; s < m; s++) {
					sum = 0;
					for (j = 0; j < k; j++)
						sum += arr1[i][j] * arr2[j][s];
					result[i][s] = sum;
				}
			}
			return result;
	}
