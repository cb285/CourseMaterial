/* Clayton Brutus
   Jacob Ball
   
   CS470
   Memory Management Project
 */

#include <iostream>       // cout
#include <fstream>        // ifstream

#include <list>           // linked list datatype
#include <unordered_set>  // unordered set datatype
#include <map>            // map datatype
#include <vector>         // vector datatype

using namespace std;

// Struct: Page
// represents a page
// stores PID and pageRef numbers
struct Page
{   
   int currPID;
   int currPageRef;
   
   // constructor with default values
   Page (int PID = -1, int pageRef = -1)
   {
      currPID = PID;
      currPageRef = pageRef;
   }
};

// hash function for Page struct:
namespace std
{
   template <>
   struct hash<Page>
   {
      size_t operator()(const Page& p) const
      {
	 // calculate and return hash value
	 return p.currPID*31 + p.currPageRef;
      }
   };
}

// Function Prototypes:
bool operator== (const Page& p1, const Page& p2);

void ReadRefString (ifstream& inputFile, vector<Page>& l);

int FIFOsim (const vector<Page>& refStr, unsigned int numPages);
int LRUsim (const vector<Page>& refStr, unsigned int numPages);
int OPTsim (const vector<Page>& refStr, unsigned int numPages);

void RemoveLast (list<Page>& l, const Page& p);
Page FindOPTReplace (int curr,  const vector<Page>& refStr, unordered_set<Page> set);
int GetDistToNext(int curr, const vector<Page>& refStr, const Page& p);

// Function: main
// gets command line args, runs simulations, prints results
int main (int argc, char* argv[])
{
   // check number of arguments given
   if (argc != 3)
   {
      cout << "USAGE: " << argv[0] << " numPages inputFile\n";
      return -1;
   }

   int numPages = atoi(argv[1]); // get number of pages

   // open input file
   ifstream inputFile;
   inputFile.open(argv[2]);

   // check if could open input file
   if (!inputFile.is_open())
   {
      cout << "Unable to open input file\n";
      return -1;
   }

   // create reference string vector
   vector<Page> refStr = vector<Page>();

   // read reference string from input file
   ReadRefString (inputFile, refStr);

   // run each simulation and print results
   cout << "FIFO: " << FIFOsim(refStr, numPages) << endl;
   cout << "LRU:  "  << LRUsim(refStr, numPages) << endl;
   cout << "OPT:  "  << OPTsim(refStr, numPages) << endl;
}

// Function: operator==
// compares two pages for equality
bool operator==(const Page& p1, const Page& p2)
{
   // check if members of each struct are equal
   return ((p1.currPID == p2.currPID) && (p1.currPageRef == p2.currPageRef));
}

// Function: RemoveLast
// given a list of pages and a page,
// removes the equivalent page that is nearest to the end of the list
void RemoveLast (list<Page>& l, const Page& p)
{
   // for each element in list, starting at end
   for (auto it = l.rbegin(); it != l.rend(); it++)
   {
      // if match
      if (*it == p)
      {
	 // erase it
	 l.erase(std::next(it).base() );
	 return; // exit function
      }
   }
}

// Function: FindOPTReplace
// given current index in reference string, a reference string, and set of current pages
// returns the page that should be replaced
Page FindOPTReplace (int curr,  const vector<Page>& refStr, unordered_set<Page> set)
{
   // create var for distance from current index
   int dist = 0;

   // create vector for storing distances
   vector<int> distances = vector<int>();

   // create map for mapping distances to pages
   map<int, Page> distMap = map<int, Page>();

   // for each element in set of current pages
   for (auto sIt = set.begin(); sIt != set.end(); sIt++)
   {
      // get distance to next occurence of this page
      dist = GetDistToNext(curr, refStr, *sIt);
      // add distance to list
      distances.push_back(dist);
      // add distance and page to map
      distMap.insert(pair<int, Page>(dist, *sIt));
   }

   // if there's a page that won't be used in future
   if (distMap.count(-1) > 0)
      return distMap.at(-1); // return page that won't ever be used again

   // find maximum distance
   int maxDist = distances.at(0);
   
   for (unsigned int i = 1; i < distances.size(); i++)      
      if (distances.at(i) > maxDist)
	 maxDist = distances.at(i);

   return distMap.at(maxDist); // return page with max distance to next use
}

// Function: GetDistToNext
// given current index, reference string, and a page
// counts the distance to the next occurence of this page
int GetDistToNext(int curr, const vector<Page>& refStr, const Page& p)
{
   // var for counting distance
   int dist = 0;

   // from current index to end of reference string
   for (unsigned int i = curr; i < refStr.size(); i++)
   {
      // if matches page
      if (refStr.at(i) == p)
	 return dist; // return distance to next occurance

      // if doesn't match, add 1 to distance
      dist++;
   }
   
   return -1; // if not found (never used again), return -1
}

// Function: ReadRefString
// given input file stream, passes back reference string as a vector
void ReadRefString (ifstream& inputFile, vector<Page>& l)
{
   // clear ref string
   l.clear();

   // vars for storing page info
   int PID = 0, pageRef = 0;

   // add each page to the reference string
   while ((inputFile >> PID) && (inputFile >> pageRef))
      l.push_back(Page(PID, pageRef));

   // close the input file stream
   inputFile.close();
}

// Function: FIFOsim
// given string reference and num of pages,
// runs FIFO algorithm and returns number of page faults
int FIFOsim (const vector<Page>& refStr, unsigned int numPages)
{
   list<Page> pageDeque = list<Page>(); // create queue of pages
   unordered_set<Page> pageSet = unordered_set<Page>(); // create list of pages
   
   int FIFOFaults = 0; // create var for counting faults   
   
   // for each entry in input file:
   for (unsigned int i = 0; i < refStr.size(); i++)
   {            
      // if not a page fault, do nothing
      if (pageSet.count(refStr.at(i)))
	 continue; // go to next entry in input file
      
      // otherwise, is a page fault
      FIFOFaults++;
      
      // if all pages have been used:
      if (pageSet.size() == numPages)
      {
	 pageSet.erase(pageDeque.back()); // remove oldest page from set
	 pageDeque.pop_back(); // remove oldest page from queue
      }
      
      pageSet.insert(refStr.at(i)); // add to set of current pages
      pageDeque.push_front(refStr.at(i)); // put on front of FIFO queue
   }
   
   return FIFOFaults;
}

// Function: LRUsim
// given string reference and num of pages,
// runs LRU algorithm and returns number of page faults 
int LRUsim (const vector<Page>& refStr, unsigned int numPages)
{
   list<Page> pageDeque = list<Page>(); // create queue of pages
   unordered_set<Page> pageSet = unordered_set<Page>(); // create list of pages
   
   int LRUFaults = 0; // create var for counting faults
   
   // for each entry in reference string:
   for (unsigned int i = 0; i < refStr.size(); i++)
   {      
      pageDeque.push_front(refStr.at(i)); // add to front of queue (most recently used)
      
      // if page is already loaded (not a page fault):
      if (pageSet.count(refStr.at(i)) > 0)
      {
	 RemoveLast(pageDeque, refStr.at(i)); // remove duplicate page from queue
	 continue; // go to next entry in input file
      }
      
      // otherwise, is a page fault:
      
      LRUFaults++; // increment page fault counter
      
      // if all pages have been used:
      if (pageSet.size() == numPages)
      {
	 pageSet.erase(pageDeque.back()); // remove least recently used page from set
	 pageDeque.pop_back(); // remove lease recently used page from queue 
      }
      
      pageSet.insert(refStr.at(i)); // add to set of current pages
   }

   // return number of page faults
   return LRUFaults;
}

// Function: OPTsim
// given string reference and num of pages,
// runs OPT algorithm and returns number of page faults
int OPTsim (const vector<Page>& refStr, unsigned int numPages)
{
   list<Page> pageDeque = list<Page>(); // create queue of pages
   unordered_set<Page> pageSet = unordered_set<Page>(); // create list of pages
   
   int OPTFaults = 0; // create var for counting faults
   
   // for each entry in reference string:
   for (unsigned int i = 0; i < refStr.size(); i++)
   {            
      // if page is already loaded (not a page fault):
      if (pageSet.count(refStr.at(i)) > 0)
	 continue; // go to next entry in input file
      
      // otherwise, is a page fault:
      
      OPTFaults++; // increment page fault counter
      
      // if all pages have been used:
      if (pageSet.size() == numPages)
	 pageSet.erase(FindOPTReplace(i, refStr, pageSet));
      
      pageSet.insert(refStr.at(i)); // add to set of current pages
   }
   
   // return number of page faults
   return OPTFaults;
}
