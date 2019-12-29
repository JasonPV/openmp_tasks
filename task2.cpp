#include <iostream>
#include <ctime>
#include <omp.h>
#include <chrono>
using namespace std;
int** multiplication_single(int** arr1, int** arr2, int n, int m, int k);
int** multiplication_multy(int** arr1, int** arr2, int n, int m, int k);
void print_arr(int** arr, int n, int m);

int main()
{
	srand(time(0));
	int n = 100;
	int m = 1000;
	int k = 100;

	int** arr1 = new int*[n];
	int** arr2 = new int*[m];

	for (int i = 0; i < n; i++)
		arr1[i] = new int[m];

	for (int i = 0; i < m; i++)
		arr2[i] = new int[k];
	
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			arr1[i][j] = rand() % 10;

	for (int i = 0; i < m; i++)
		for (int j = 0; j < k; j++)
			arr2[i][j] = rand() % 10;

	auto start1 = chrono::system_clock::now();
	int** result1 = multiplication_single(arr1, arr2, n, m, k);
	auto end1 = chrono::system_clock::now();

	auto start2 = chrono::system_clock::now();
	int** result2 = multiplication_multy(arr1, arr2, n, m, k);
	auto end2 = chrono::system_clock::now();
	
	cout << "single thread: " << endl;
	cout << "time = " << chrono::duration_cast<chrono::microseconds>(end1 - start1).count()<< endl;
	print_arr(result1, n, k);

	cout << "multy threads: " << endl;
	cout << "time = " << chrono::duration_cast<chrono::microseconds>(end2 - start2).count() << endl;
	print_arr(result2, n, k);

	return 0;
}

int** multiplication_single(int** arr1, int** arr2, int n, int m, int k)
{
	int** result = new int* [n];
	for (int i = 0; i < n; i++)
		result[i] = new int[k];

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < k; j++)
		{
			result[i][j] = 0;
			for (int s = 0; s < m; s++)
			{
				result[i][j] += arr1[i][s] * arr2[s][j];
			}
		}
	}
	return result;
}

int** multiplication_multy(int** arr1, int** arr2, int n, int m, int k)
{
	omp_set_num_threads(8);
	int** result = new int* [n];
	for (int i = 0; i < n; i++)
		result[i] = new int[k];
	omp_set_num_threads(4);
	int i, j, s;
	#pragma omp parallel for shared(arr1, arr2, result) private(i, j, s) 
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < k; j++)
		{
			result[i][j] = 0;
			for (s = 0; s < m; s++)
			{
				result[i][j] += arr1[i][s] * arr2[s][j];
			}
		}
	}
	return result;
}

void print_arr(int** arr, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			cout << arr[i][j] << " ";
		cout << endl;
	}
}
