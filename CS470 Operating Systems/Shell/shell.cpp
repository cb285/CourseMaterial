// Clayton Brutus
// CS470 - Dr. Hwang
// Shell Project
// Due: 2/3/17

#include <iostream> // cin, cout
#include <unistd.h> // fork, execvp, getcwd, chdir
#include <sys/types.h> // pid_t
#include <sys/wait.h> // wait
#include <string> // string
#include <cstring> // strcpy
#include <sstream> // istringstream
#include <vector> // vector
#include <climits> // INT_MAX

using namespace std;

#define MAXHIST 10 // number of commands stored in history
#define MAXPATH 100 // max length of cwd

string getCurrWorkingDir();
vector<string> getInput();
void printHistory(int curr, vector<vector<string>>* history);
char** convert(vector<string>* vect);

int main()
{
   vector<vector<string>> history; // stores last 10 commands
   vector<string> input; // stores user input
   int curr = 0; // current command number
   string cwd = ""; // current working directory
   bool inBack = false; // bool for running in background or not
   
   while(1)
   {
      inBack = false;
      
      // get user input
      input = getInput();

      // if empty do nothing
      if (input.size() == 0)
	 continue;
      
      // check for "exit"
      else if (input.at(0) == "exit")
	 // terminate program
	 exit(0);
      
      // check for "history"
      else if (input.at(0) == "history")
      {
	 // display history
	 printHistory(curr, &history);
	 
	 // done
	 continue;
      }
      
      // check for "cd"
      else if (input.at(0) == "cd")
      {
	 if(input.size() != 2)
	    cout << "USAGE: cd <directory>\n";
	 
	 else if(chdir(input.at(1).c_str()))
	    cout << "invalid directory\n";
	 
	 // done
	 continue;
      }
      
      // check for "pwd"
      else if (input.at(0) == "pwd")
      {
	 if (input.size() != 1)
	 {
	    cout << "USAGE: pwd\n";
	    continue;
	 }
	 
	 cout << getCurrWorkingDir() << "\n";
	 
	 // done
	 continue;
      }
      
      // check if "!!"
      else if (input.at(0) == "!!")
      {
	 // display error message if history is empty
	 if (history.size() == 0)
	 {
	    cout << "No commands in history\n";
	    continue;
	 }
	 else
	    // change input to most recent command
	    input = history.back();
      }
      
      // check if "!N", where N is an integer
      else if (input.at(0)[0] == '!')
      {
	 // copy string
	 string str = input.at(0);
	 
	 // remove '!'
	 str.erase(0, 1);
	 
	 int N = 0;
	 
	 // try to parse number
	 try {
	    // parse integer
	    N = stoi(str);
	 }
	 catch(const std::invalid_argument& e)
	 {
	    // display message if couldn't parse
	    cout << "Error: invalid command number\n";
	    continue;
	 }
	 
	 // check if valid
	 if (N > curr || N <= (curr - (int)history.size()))
	 {
	    std::cout << "No such command in history\n";
	    continue;
	 }
	 
	 // change input to N'th command
	 input = history.at(N - curr + history.size() - 1);
      }
      
      // increment current command number
      curr++;
      
      // add to history
      history.push_back(input);
      
      // if history contains more than MAXHIST elements, then remove the oldest
      if (history.size() > MAXHIST)
	 history.erase(history.begin());
      
      // see if args contain '&'
      for (auto i = input.begin(); i != input.end(); i++)
      {
      	 // run in back if contains '&'
	 if (*i == "&")
	 {
	    inBack = true;
	    input.erase(i);
	    break;
	 }
      }
      
      // convert arguments to C strings for passing to execvp function
      char** args = convert(&input);
      
      // fork to create child process
      pid_t pid;
      pid = fork();
      
      // if failed:
      if (pid < 0)
      {
	 // display message and exit
	 cout << "Error: fork failed\n";
	 exit(1);
      }
      
      // if child:
      else if (pid == 0)
      {
	 // if contained '&', detach process from shell program
	 if (inBack)
	 {
	    // fork again to create grandchild process
	    pid = fork();
	    
	    // if failed:
	    if (pid < 0)
	    {
	       cout << "Error: fork failed\n";
	       exit(1);
	    }
	    
	    // if grandchild:
	    else if (pid == 0)
	    {
	       // execute command and let init adopt process
	       if (execvp(args[0], args) < 0)
	       {
		  // if failed: display message and exit
		  cout << "Error: exec failed\n";
		  exit(1);
	       }
	    }
	    
	    // if child:
	    else
	    {
	       // free memory
	       for(size_t i = 0; i < input.size(); i++)
		  delete [] args[i];
	       
	       delete [] args;
	       
	       // terminate child
	       exit(0);
	    }
	    
            // reset in background bool
	    inBack = false;
	 }
	 
	 // if didn't have '&', then run in child process
	 else
	 {
	    // execute command
	    if (execvp(args[0], args) < 0)
	    {
	       // if failed: display message and exit
	       cout << "Error: exec failed\n";
	       exit(1);
	    }
	 }
      }
      
      // if parent:
      else
      {
	 // wait for child exec to finish
	 wait(NULL);

	 // free memory
	 for(size_t i = 0; i < input.size(); i++)
	    delete [] args[i];
	 
	 delete [] args;
      }
   } // end of while(1) loop

   return 0;
}

// Function: getCurrWorkingDir
// returns the current working directory as a string
string getCurrWorkingDir()
{
   // temporary variable
   char temp[MAXPATH];

   // get CWD and return string
   return getcwd(temp, MAXPATH);
}

// Function: getInput
// shows the prompt, gets user input and returns the arguments
vector<string> getInput()
{
   string line; // for storing user input

   // storage for arguments
   vector<string> str_args;
   
   // show prompt
   cout << "osh> ";
   
   // read input
   getline(cin, line);

   // if didn't enter anything, then return empty vector
   if (line.empty())
   {
      return str_args;
   }
   
   // create string stream
   istringstream iss(line);
   
   string word; // for storing each argument
   
   // read each argument
   while (iss >> word)
      // store arg
      str_args.push_back(word);

   return str_args;
}

// Function: printHistory
// input: current commmand number, vector of history (by reference)
void printHistory(int curr, vector<vector<string>>* history)
{
   // if haven't executed any commands, then do nothing
   if (curr == 0)
      return;
      
   // for each command in history
   for (int x = history->size() - 1; x >= 0; x--)
   {
      // write command number
      cout << curr << " ";
      // write all args
      for (string arg : history->at(x))
	 cout << arg << " ";
      
      // new line
      cout << "\n";

      // decrement command number
      curr--;
   }
}

// Function: convert
// converts vector of C++ strings to array of C strings
char** convert(vector<string>* vect)
{
   // create storage for C strings
   char** args = new char*[vect->size() + 1];

   // copy each string
   for(size_t i = 0; i < vect->size(); i++)
   {	 
      args[i] = new char[vect->at(i).size() + 1];
      strcpy(args[i], vect->at(i).c_str());
   }
   
   // must be NULL terminated
   args[vect->size()] = NULL;

   // return array of C strings
   return args;
}
