#include <iostream>
#include <ctime>
#include <omp.h>
#include <chrono>

using namespace std;

int get_min(int* line, int m, bool is_par);
int get_max(int** arr, int n, int m, bool is_par);

int main()
{ 
	srand(time(0));
	const int n = 1000;
	const int m = 10000;
	int** arr = new int* [n];
	for (int i = 0; i < n; i++)
		arr[i] = new int[m];


	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			arr[i][j] = rand() % 1000;
	
	auto start1 = std::chrono::system_clock::now();
	int result1 = get_max(arr, n, m, false);
	auto end1 = std::chrono::system_clock::now();

	auto start2 = std::chrono::system_clock::now();
	int result2 = get_max(arr, n, m, true);
	auto end2 = std::chrono::system_clock::now();


	cout << "For single thread:" << endl << "result = " << result1 << endl << "time = " << chrono::duration_cast<chrono::microseconds>(end1 - start1).count() << endl;
	cout << "For multy thread:" << endl << "result = " << result2 << endl << "time = " << chrono::duration_cast<chrono::microseconds>(end2 - start2).count() << endl;
	return 0;
}

int get_min(int* line, int m, bool is_par)
{
	int min = line[0];

	for (int i = 0; i < m; i++)
	{
		if (line[i] < min)
			min = line[i];
	}
	return min;
}

int get_max(int** arr, int n, int m, bool is_par)
{
	int* min_of_lines = new int[n];
#pragma omp parallel if(is_par)
	{
#pragma omp for
		for (int i = 0; i < n; i++)
			min_of_lines[i] = get_min(arr[i], m, is_par);
	}

	int max = min_of_lines[0];
#pragma omp parallel for 
	for (int i = 1; i < n; i++)
		if (min_of_lines[i] > max)
			max = min_of_lines[i];
	return max;
}
