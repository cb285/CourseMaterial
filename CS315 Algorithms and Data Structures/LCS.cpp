#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>

using namespace std;

// function prototypes:
void LCSLength (const string& x, const string& y, vector <vector<int> >& c, vector < vector<char> >& b);
string GetLCS(const vector <vector<char> >& b, string x, int i, int j);
void PrintTable (const string& x, const string& y, const vector <vector<int> >& c, const vector < vector<char> >& b);

int main (int argc, char* argv[])
{
   // check number of arguments
   if (argc != 2)
   {
      cout << "USAGE: " << argv[0] << " filename" << endl;
      return -1;
   }

   // open input file
   ifstream inputFile;

   inputFile.open (argv[1]);
   
   // check if open succeeded
   if (!inputFile.is_open())
   {
      cout << "couldn't open input file!" << endl;
      return -1;
   }

   // read strings
   string x;
   string y;
   
   inputFile >> x;
   inputFile >> y;
   
   vector< vector<int> > c;
   vector< vector<char> > b;

   // create tables c and b
   LCSLength(x, y, c, b);

   // print LCS
   cout << "LCS is:" << endl;
   cout << GetLCS(b, x, x.length() - 1, y.length() - 1) << endl;
}

// Function: LCSLength
// given 2 strings, passes back tables c and b
void LCSLength (const string& x, const string& y, vector <vector<int> >& c, vector < vector<char> >& b)
{
   // get lengths
   int m = x.length();
   int n = y.length();

   // create tables
   c = vector < vector<int> > (m+1, vector<int>(n+1));
   b = vector < vector<char> >(m, vector<char>(n));

   // do lcs-length algorithm
   for (int i = 1; i <= m; i++)
      for (int j = 1; j <= n; j++)
      {	 
	 if (x[i-1] == y[j-1])
	 {
	    c[i][j] = c[i-1][j-1] + 1;
	    b[i-1][j-1] = '\\';
	 }
	 
	 else if (c[i-1][j] >= c[i][j-1])
	 {
	    c[i][j] = c[i-1][j];
	    b[i-1][j-1] = '^';
	 }
	 
	 else
	 {
	    c[i][j] = c[i][j-1];
	    b[i-1][j-1] = '<';
	 }
      }
}

// Function: GetLCS
// uses a non-recursive version on LCS-Print algorithm
// given table b, string x, indexes of last elements of strings
// returns LCS as a string
string GetLCS(const vector <vector<char> >& b, string x, int i, int j)
{
   list<char> charList = list<char>();

   while (i >= 0 && j >= 0)
   {  
      if (b[i][j] == '\\')
      {
	 charList.push_front(x[i]);
	 i--;
	 j--;
      }
      else if (b[i][j] == '^')
	 i--;
      else
	 j--;
   }
   
   string result;
   
   for (auto it = charList.begin(); it != charList.end(); it++)
      result.push_back(*it);
   
   return result;
}

