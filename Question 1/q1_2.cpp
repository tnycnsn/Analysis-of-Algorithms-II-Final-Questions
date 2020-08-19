#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

void print_arr(vector<int>& arr, int first, int last)
{
	for (int i = first; i <= last; i++)
	{
		string if_eol = (i == last) ? "\n" : ", ";

		cout << arr[i] << if_eol;
	}
}

void func (vector<int>& arr, int first, int last){
	int m = 0;
	int k = first;

	while (k < last){
		if (arr[k] > arr[k+1]){	//swap operation
			int temp = arr[k];
			arr[k] = arr[k+1];
			arr[k+1] = temp;
		} else k += 2;	//iterate to the next position odd-even
	}

	vector<int> A;
	for (k = first+1; k <= last; k+=2){
		A.push_back(arr[k]);
		m++;
	}
	int x = first; k = first;
	while (k <= last){
		arr[x++] = arr[k];
		k+=2;
	}
	for(int k = 0; k < m; k++){
		arr[x++] = A[k];
	}
}

void merge (vector<int>& arr, int first, int mid, int last){
	int i = first;
	int j = mid+1;

	vector<int> temp;
	while (i <= mid && j <= last){
		if (arr[i] < arr[j]) temp.push_back(arr[i++]);
		else temp.push_back(arr[j++]);
	}
	while (i <= mid) temp.push_back(arr[i++]);
	while (j <= last) temp.push_back(arr[j++]);

	i = first;
	for (int ind = 0; ind < temp.size(); ind++) arr[i++] = temp[ind];
}

void sort(vector<int>& arr, int first, int last){
	int n = last-first+1;
	if (n <= 1) return;
	else {
		func(arr, first, last);
		int mid = (first+last)/2;
		sort(arr, first, mid);
		sort(arr, mid+1, last);
		merge(arr, first, mid, last);
	}
}

int minDC(vector<int>& arr, int first, int last){
	//cout << "first: " << first << " last: " << last << endl;
	int n = last-first+1;
	if (n <= 1) return first;	//return index of the item
	else{
		int mid = (first+last)/2;
		int lowerMin = minDC(arr, first, mid);
		int upperMin = minDC(arr, mid+1, last);
		if (arr[lowerMin] < arr[upperMin]) return lowerMin;
		else return upperMin;
	}
}

int maxDC(vector<int>& arr, int first, int last){
	//cout << "first: " << first << " last: " << last << endl;
	int n = last-first+1;
	if (n <= 1) return first;	//return index of the item
	else{
		int mid = (first+last)/2;
		int lowerMax = maxDC(arr, first, mid);
		int upperMax = maxDC(arr, mid+1, last);
		if (arr[lowerMax] > arr[upperMax]) return lowerMax;
		else return upperMax;
	}
}

void MinMaxSort(vector<int>& arr, int first, int last){
	int i = first;
	int j = last;
	cout << "Min_max unsorted: ";
	print_arr(arr, first, last);

	while (i < j){	//the search area shrinking in by 2 in with each iteration

		int min = minDC(arr, i, j);
		int temp = arr[i];	//i is the first element of the shrinking array in each iteration
		arr[i] = arr[min];
		arr[min] = temp;

		int max = maxDC(arr, i, j);
		temp = arr[j];	//j is the last element of the shrinking array in each iteration
		arr[j] = arr[max];
		arr[max] = temp;

		cout << "Min: " << arr[min] << " " << "Max: " << arr[max] << endl;

		i++; j--;
	}

	cout << "Min_max sorted: ";
	print_arr(arr, first, last);
}

void frankensteinSort(vector<int>& arr, int first, int last){
	int n = last-first+1;
	if (n <= 1) return;
	else {
		print_arr(arr, first, last);
		func(arr, first, last);
		int mid = (first+last)/2;
		MinMaxSort(arr, first, mid);	//left sub-array is sorted by min-max
		frankensteinSort(arr, mid+1, last);				//right sub-array is sorted by EMS
		merge(arr, first, mid, last);
	}
}

int main(int argc, char *argv[])
{

	vector<int> all_values;


	try {

		string filename = argv[1];

		ifstream myfile(filename);


		if (myfile.is_open())
		{
			string line;
			while (getline(myfile, line))
			{
				int number = stoi(line);
				all_values.push_back(number);
			}

			//sort(all_values, 0, all_values.size() - 1);
			//MinMaxSort(all_values, 0, all_values.size() - 1);
			frankensteinSort(all_values, 0, all_values.size() - 1);

			print_arr(all_values, 0, all_values.size() - 1);
		}
		else
		{
			cout << "File is not available." << endl;
			throw;
		}

	}
	catch (const ifstream::failure& e)
	{
		cerr << "Exception opening the file!" << endl;
		return -1;
	}
	catch (...)
	{
		cerr << "Something's wrong!" << endl;
		return -1;
	}



	return 0;
}
