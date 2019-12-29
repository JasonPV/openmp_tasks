#include <iostream>
#include <ctime>
#include <chrono>
#include <omp.h>

using namespace std;

int* mult_row(int** arr, int* vec, int n, int m, int threads_num);
int* mult_col(int** arr, int* vec, int n, int m, int threads_num);
int* mult_block(int** arr, int* vec, int n, int m, int threads_num);
int* mult(int** arr, int* vec, int n, int m);



int main()
{
	for (int threads_number = 1; threads_number < 17; threads_number*=2)
	{
		cout << "Threads number: " << threads_number << endl;
		for (int dim = 10; dim < 100000; dim *= 10)
		{
			srand(time(0));
			int n = dim;
			int m = dim;
			int** arr = new int* [n];
			int* vec = new int[m];
			for (int i = 0; i < n; i++)
				arr[i] = new int[m];

			for (int i = 0; i < n; i++)
				for (int j = 0; j < m; j++)
					arr[i][j] = rand() % 100;

			for (int i = 0; i < m; i++)
				vec[i] = rand() % 100;

			auto start1 = chrono::system_clock::now();
			int* result1 = mult_row(arr, vec, n, m, threads_number);
			auto end1 = chrono::system_clock::now();
			cout << "for row:" << endl << "dim = " << dim << endl << "threads_number = " << threads_number << endl << "time = ";
			cout << chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count() << endl;

			auto start2 = chrono::system_clock::now();
			int* result2 = mult_col(arr, vec, n, m, threads_number);
			auto end2 = chrono::system_clock::now();
			cout << "for col:" << endl << "dim = " << dim << endl << "threads_number = " << threads_number << endl << "time = ";
			cout << chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count() << endl;

			auto start3 = chrono::system_clock::now();
			int* result3 = mult_block(arr, vec, n, m, threads_number);
			auto end3 = chrono::system_clock::now();
			cout << "for block:" << endl << "dim = " << dim << endl << "threads_number = " << threads_number << endl << "time = ";
			cout << chrono::duration_cast<std::chrono::microseconds>(end3 - start3).count() << endl;
		}
	}

	return 0;
}

int* mult_row(int** arr, int* vec, int n, int m, int threads_num)
{
	omp_set_num_threads(threads_num);
	int* result = new int[n];
	for (int i = 0; i < n; i++)
		result[i] = 0;
	int i, j;
#pragma omp parallel for private(j)
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
			result[i] += arr[i][j] * vec[j];
	}
	return result;
}

int* mult_col(int** arr, int* vec, int n, int m, int threads_num)
{
	omp_set_num_threads(threads_num);
	int* result = new int[n];
	for (int i = 0; i < n; i++)
		result[i] = 0;
	int i, j;
	int glsum = 0;
	for (i = 0; i < n; i++)
	{
		glsum = 0;
#pragma omp parallel for reduction(+:glsum)
		for (j = 0; j < m; j++)
			glsum += arr[i][j] * vec[j];
		result[i] = glsum;
	}
	return result;
}

int* mult_block(int** arr, int* vec, int n, int m, int threads_num)
{
	omp_set_num_threads(threads_num);
	int* result = new int[n];
	for (int i = 0; i < n; i++)
		result[i] = 0;

#pragma omp parallel shared(arr, vec, result) 
	{
		int s = omp_get_num_threads();
		int q = s;
		int block_height = n / s;
		int block_width = m / q;

#pragma omp for
		for (int block_count = 0; block_count < s * q; block_count++)
		{
			int i = block_count / q;
			int j = block_count % q;
			for (int k = i * block_height; k < (i + 1) * block_height; k++)
				for (int l = j * block_width; l < (j + 1) * block_width; l++)
					result[k] += arr[k][l] * vec[l];
		}
	}
	return result;
	
	return nullptr;
}

int* mult(int** arr, int* vec, int n, int m)
{
	int* result = new int[n];
	for (int i = 0; i < n; i++)
	{
		result[i] = 0;
		for (int j = 0; j < n; j++)
		{
			result[i] += arr[i][j] * vec[j];
		}
	}
	return result;
}
