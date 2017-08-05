// Clayton Brutus
// Deterministic Select

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int Select(vector<int>& numList, int p, int r, int i);
int Partition(vector<int>& numList, int p, int r, int x);
void InsertionSort(vector<int>& numList, int p, int r);

int main (int argc, char* argv[])
{
   // check number of arguments
   if (argc != 2)
   {
      cout << "USAGE: " << argv[0] << " inputFile\n";
      return -1;
   }
   
   // open input file
   ifstream inputFile;
   
   inputFile.open(argv[1]);

   // check if could open input file
   if (!inputFile.is_open())
   {
      cout << "Unable to open input file\n";
      return -1;
   }
   
   int n = 0, i = 0;
   
   inputFile >> n; // get number of elements in list
   
   inputFile >> i; // get order statistic
   
   int num = 0;
   
   vector<int> numList = vector<int>();
   
   numList.reserve(n); // resize vector to hold all numbers
   
   while (inputFile >> num) // read each number from file
      numList.push_back(num); // add number to list
   
   cout << "The " << i << "th order statistic is: " << Select(numList, 0, numList.size(), i-1) << endl;
}

int Select(vector<int>& numList, int p, int r, int i)
{   
   // if 5 or less numbers in list, use insertion sort
   if (r - p <= 5)
   {
      InsertionSort(numList, p, r);
      return numList.at(i);
   }
   
   vector<vector<int>> listsOf5 = vector<vector<int>>();

   // create groups of 5 numbers
   for (int j = p; j < r; j++)
   {
      if ((j-p) % 5 == 0)
	 listsOf5.push_back(vector<int>());
      
      listsOf5.back().push_back(numList.at(j));
   }
      
   vector<int> listOfMedians = vector<int>();
   
   // find median of each group of 5
   for (unsigned int j = 0; j < listsOf5.size(); j++)
      listOfMedians.push_back(Select(listsOf5.at(j), 0, listsOf5.at(j).size(), listsOf5.at(j).size() / 2)); // find and add median to other list
   
   int x = Select(listOfMedians, 0, listOfMedians.size(), listOfMedians.size() / 2); // find index of median of medians
      
   int q = Partition(numList, p, r, x); // parition around median of medians
   // q is the found order statistic
   
   // if found the wanted order statistic
   if (i == q)
      return numList.at(i); // return the order statistic
   
   // if order stat is to left of found one
   else if (i < q)
      return Select(numList, p, q, i); // recurse on left part of list
   
   // if order stat is to right of found one
   else
      return Select(numList, q+1, r, i); // recurse on right part of list
}

int Partition(vector<int>& numList, int p, int r, int pivotValue)
{   
   int pivotIndex;

   // find pivot index
   for (int i = p; i < r; i++)
      if (numList.at(i) == pivotValue)
      {
	 pivotIndex = i;
	 break;
      }

   // put pivot value at end
   swap (numList.at(pivotIndex), numList.at(r-1));

   // partition
   int i = p - 1;
   
   for (int j = p; j < r - 1; j++)
   {
      if (numList.at(j) <= pivotValue)
      {
	 i++;
	 swap(numList.at(i), numList.at(j));
      }
   }

   swap (numList.at(i+1), numList.at(r-1));   
   
   // return index of pivot
   return i+1;
}

void InsertionSort(vector<int>& numList, int p, int r)
{
   // use insertion sort algorithm
   int j = p;
   
   for (int i = p + 1; i < r; i++)
   {
      j = i;
      
      while (j > 0 && numList.at(j-1) > numList.at(j))
      {
	 swap(numList.at(j), numList.at(j-1));
	 j--;
      }
   }
}
