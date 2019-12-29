#include <iostream>
#include <chrono>
#include <fstream>
#include <omp.h>

using namespace std;

int search_substring(string str, string substr, bool is_parallel);

int main()
{
	ifstream file("in.txt");
	string str;
	string substr = "hel";
	char c;

	while (!file.eof())
	{
		file.get(c);
		str.push_back(c);
	}
	file.close();
	

	auto start1 = std::chrono::system_clock::now();
	int result1 = search_substring(str, substr, false);
	auto end1 = std::chrono::system_clock::now();

	auto start2 = std::chrono::system_clock::now();
	int result2 = search_substring(str, substr, true);
	auto end2 = std::chrono::system_clock::now();

	cout << "for single: " << endl;
	cout << "result = " << result1 << endl << "time = " << chrono::duration_cast<chrono::microseconds>(end1 - start1).count() << endl;

	cout << "for multy: " << endl;
	cout << "result = " << result2 << endl << "time = " << chrono::duration_cast<chrono::microseconds>(end2 - start2).count() << endl;
	return 0;
}

int search_substring(string str, string substr, bool is_parallel)
{
	int result= -1;
#pragma omp parallel shared(str, substr) if(is_parallel)
	{
#pragma omp for
		for (int i = 0; i < str.length() - substr.length() + 1; i++)
		{
			int k = 0;
			for (int j = 0; j < substr.length(); j++)
			{
				if (str[i + j] != substr[j])
					break;
				k++;
			}
			if (k == substr.length())
			{
				result = i;
			}
		}
	}
	return result;
}
