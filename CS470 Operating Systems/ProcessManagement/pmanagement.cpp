// Clayton Brutus
// CS470 - Operating Systems
// Dr. Hwang
// Process Management Project

#include <iostream> // cout
#include <fstream>  // ifstream, ofstream
#include <list>     // linked list datatype (used for queues)

#define MAXCHILDREN 20 // max number of children for any process

using namespace std;

// custom class: Process
// program's representation of a process
class Process
{
   int PID; // Process ID
   int burstTimeInit; // intitial burst time
   int burstTimeRemain; // current burst time remaining
   int qTimeInit; // initial Q time
   int qTimeRemain; // Q time remaining
   
   string status; // current state
   
   int parentPID; // parent Process ID
   list<Process*> children; // list of pointers to children processes
   
   int EID; // event ID if (waiting on one)
   
public:   
   Process(int parentID = 0, int processID = 0, int burstTime = 0, int qTime = 0)
   {
      parentPID = parentID; // set parent ID
      PID = processID; // set PID
      burstTimeRemain = burstTime; // set init burst time
      burstTimeInit = burstTime;   // set current burst time
      qTimeInit = qTime; // set init Q time
      
      EID = -1; // eid = -1 (not waiting on an event)
      
      children = list<Process*>(); // create empty children list
      
      qTimeRemain = 0; // set current Q time to 0
      status = "ready"; // set status to ready
   }
   
   int GetPID() const
   {
      return PID; // return process ID
   }
   
   int GetParentPID() const
   {
      return parentPID; // return parent process ID
   }

   list<Process*> GetChildren() const
   {
      return children; // return list of pointers to children
   }
   
   void AddChild(Process* childProc)
   {
      children.push_front(childProc); // add child PID to list
   }
   
   int GetBurstTime() const
   {
      return burstTimeRemain; // return remaining burst time
   }

   int GetInitBurstTime() const
   {
      return burstTimeInit; // return initial burst time
   }

   int GetQTime() const
   {
      return qTimeRemain; // return remaining Q time
   }
   
   void Start()
   {
      qTimeRemain = qTimeInit; // set remaining time to init
      
      status = "running";
   }
   
   void Tick()
   {
      // subtract 1 from burst time
      burstTimeRemain--;

      // subtract 1 from q time
      qTimeRemain--;
      
      // if terminated
      if (burstTimeRemain == 0)
	 status = "terminated";
      
      // if finished quantum
      else if (qTimeRemain == 0)
	 status = "ready";
   }
   
   void Wait(int eventID)
   {
      // save event id
      EID = eventID;
      
      // change status to wait
      status = "wait";
   }
   
   bool Event(int eventID)
   {
      // if process was waiting on this event:
      if (eventID == EID)
      {
	 // reset event id
	 EID = -1;
	 
	 // change status to ready
	 status = "ready";
	 
	 return true; // return true
      }
      
      return false; // otherwise, return false
   }
   
   int GetEID() const
   {
      return EID; // return EID
   }
};

void TerminateChildren (ofstream& outputFile, Process& proc, list<Process>& readyQueue, list<Process>& waitQueue);
void PrintStatus(ofstream& outputFile, const Process& runningProcess, const list<Process>& readyQueue, const list<Process>& waitQueue);

// Function: main
// runs process management simulation
int main(int argc, char* argv[])
{
   // if 3 arguments not given, display message and exit
   if (argc != 4)
   {
      cout << "USAGE: " << argv[0] << " Qtime inputFilename outputFilename\n";
      return -1;
   }
   
   int qTime = atoi(argv[1]); // get quantum time

   // check if Q time is positive number
   if (qTime <= 0)
   {
      cout << "ERROR: quantum time must be positive\n"; // display message and exit program
      return -1;
   }
   
   ifstream inputFile; // create input filestream

   ofstream outputFile; // create output filestream
   
   inputFile.open(argv[2]); // open input file
   outputFile.open(argv[3]); // open output file
   
   // check if could open input file
   if (!inputFile.is_open())
   {
      cout << "Unable to open input file.\n"; // display message
      inputFile.close(); // close files
      outputFile.close();
      return -1; // exit
   }

   // check if could open output file
   if (!outputFile.is_open())
   {
      cout << "Unable to open output file.\n"; // display message
      inputFile.close(); // close files
      outputFile.close(); 
      return -1; // exit
   }

   int timeElapsed = 0; // create var for total number of ticks
   
   Process runningProcess = Process(); // create PID 0 process
   Process newProcess; // create temp Process for creating processes
   
   list<Process> readyQueue = list<Process>(); // create Queues
   list<Process> waitQueue = list<Process>();
   
   char command = '0'; // create variables for storing command
   int PID = 0, burst = 0, EID = 0; // create variables for storing command arguments
   
   PrintStatus(outputFile, runningProcess, readyQueue, waitQueue); // display running process and queues
   
   inputFile >> command; // get first command
   
   while (!inputFile.eof()) // continue until reached end of file
   {
      command = toupper(command); // make commmand upper case
      
      // run the current running process for 1 tick
      runningProcess.Tick();
      timeElapsed++;
      
      switch (command)
      {
	 // create process
	 case 'C':
	    inputFile >> PID; // get PID
	    
	    inputFile >> burst; // get burst size
	    
	    outputFile << "C " << PID << " " << burst << "\n"; // display command
	    
	    newProcess = Process(runningProcess.GetPID(), PID, burst, qTime); // create new process
	    
	    runningProcess.AddChild(&newProcess); // add child PID to parent (current running process)
	    
	    readyQueue.push_back(newProcess); // add to ready queue
	    
	    outputFile << "PID " << PID << " " << burst << " placed on Ready Queue\n"; // display message
	    
	    break;
	    
	 // destroy processs
	 case 'D':
	    inputFile >> PID; // get PID
	    
	    outputFile << "D " << PID <<  "\n"; // display command
	    
	    // if is the running process:
	    if (runningProcess.GetPID() == PID)
	    {
	       TerminateChildren (outputFile, runningProcess, readyQueue, waitQueue); // terminate it and its children
	       outputFile << "PID " << PID << " " << runningProcess.GetInitBurstTime() << " terminated\n";

	       // if ready queue empty, then run PID 0
	       if (readyQueue.empty())
		  runningProcess = Process(); // PID 0
	       
	       // otherwise, get next process from ready queue
	       else
	       {
		  runningProcess = readyQueue.front(); // get next ready process
		  readyQueue.erase(readyQueue.begin()); // remove now running process from ready queue
	       }

	       // if not PID 0
	       if (runningProcess.GetPID() != 0) 
		  runningProcess.Start(); // reset Q time
	    }
	    // if it is not the running process:
	    else
	    {
	       // search for PID in Ready Queue
	       for (auto i = readyQueue.begin(); i != readyQueue.end(); i++)
		  if (PID == i->GetPID()) // if found it
		  {
		     TerminateChildren (outputFile, *i, readyQueue, waitQueue); // terminate it and its children
		     readyQueue.erase(i); // remove it from Ready Queue
		     outputFile << "PID " << i->GetPID() << " " << i->GetInitBurstTime() << " terminated\n"; // display message
		     
		     break;
		  }
	       
	       // search for PID in Wait Queue
	       for (auto i = waitQueue.begin(); i != waitQueue.end(); i++)
		  if (PID == i->GetPID()) // if found it
		  {
		     TerminateChildren (outputFile, *i, readyQueue, waitQueue); // terminate it and its children
		     waitQueue.erase(i); // remove it from Wait Queue
		     outputFile << "PID " << i->GetPID() << " terminated\n"; // display message
		     break;
		  }
	    }
	    
	    break;
	    
	 // timer interrupt
	 case 'I':
	    outputFile << "I\n"; // write command
	    break; // do nothing (ticks time above)
	 
	 // wait on event
	 case 'W':
	    if (runningProcess.GetPID() == 0)
	       break;
	    
	    inputFile >> EID; // get EID
	    
	    outputFile << "W " << EID << "\n"; // write command
	    
	    runningProcess.Wait(EID); // process waits for EID
	    
	    waitQueue.push_back(runningProcess); // add to Wait Queue

	    if (readyQueue.empty())
	       runningProcess = Process(); // PID 0
	    else
	    {
	       runningProcess = readyQueue.front(); // get next ready process
	       readyQueue.erase(readyQueue.begin()); // remove now running process from ready queue
	    }

	    if (runningProcess.GetPID() != 0)
	       runningProcess.Start(); // reset Q time
	    
	    break;
	 
	 // event happened
	 case 'E':
	    inputFile >> EID; // get EID
	    
	    outputFile << "E " << EID << "\n";
	    
	    // for each process in Wait Queue
	    for (auto i = waitQueue.begin(); i != waitQueue.end(); i++)
	       // if wait waiting on event:
	       if (i->Event(EID))
	       {		  
		  readyQueue.push_back(*i); // add to Ready Queue
		  waitQueue.erase(i);        // remove from Wait Queue
		  break;
	       }
	    
	    break;
	 
	 // exit
	 case 'X':
	    outputFile << "X\n"; // write command
	    
	    PrintStatus(outputFile, runningProcess, readyQueue, waitQueue); // display current state
	    
	    cout << "Q = " << qTime << "\n";
	    cout << "Total elasped time: " << timeElapsed << "\n";
	    
	    inputFile.close(); // close files
	    outputFile.close();
	    
	    return 0; // exit program
	    
	 default:
	    cout << "ERROR: invalid command, check input file\n";
	    return -1;
      }
      
      // if current process is finished with quantum or burst:
      if (runningProcess.GetQTime() <= 0 || runningProcess.GetBurstTime() <= 0)
      {
	 // if not finished, then put on ready queue
	 if (runningProcess.GetBurstTime() > 0 && runningProcess.GetPID() != 0)
	 {
	    readyQueue.push_back(runningProcess); // move running process to ready Queue
	 }
	 else
	    outputFile << "PID " << runningProcess.GetPID() << " " << runningProcess.GetInitBurstTime() << " terminated\n";
	 
	 if (readyQueue.empty())
	    runningProcess = Process(); // run PID 0 if nothing else ready
	 
	 // if something ready:
	 else
	 {
	    runningProcess = readyQueue.front();                   // start running first process in ready queue
	    readyQueue.erase(readyQueue.begin());                  // remove now running process from ready queue
	 }
	 
	 if (runningProcess.GetPID() != 0)
	    runningProcess.Start(); // reset Q time
      }
      
      PrintStatus(outputFile, runningProcess, readyQueue, waitQueue);
      
      outputFile << "\n";
      
      inputFile >> command; // get next command
   }
}

// Function: PrintStatus
// prints status of current running program and processes and queues
void PrintStatus(ofstream& outputFile, const Process& runningProcess, const list<Process>& readyQueue, const list<Process>& waitQueue)
{
   // don't display info about PID 0
   if (runningProcess.GetPID() == 0)
      outputFile << "PID 0 running\n";

   // otherwise display all info
   else
   {
      outputFile << "PID " << runningProcess.GetPID() << " " << runningProcess.GetBurstTime()
	   << " running with " << runningProcess.GetQTime() << " left\n";
   }

   // display ready queue
   outputFile << "Ready Queue:\n";
   
   for (auto i = readyQueue.begin(); i != readyQueue.end(); i++)
      outputFile << "PID " << i->GetPID() << " " << i->GetBurstTime() << "\n";

   // display wait queue
   outputFile << "Wait Queue:\n";
   
   for (auto i = waitQueue.begin(); i != waitQueue.end(); i++)
      outputFile << "PID " << i->GetPID() << " " << i->GetBurstTime() << " " << i->GetEID() << "\n";
}

// Function: TerminateChildren
// recursively deletes a process's children from the queue
void TerminateChildren (ofstream& outputFile, Process& proc, list<Process>& readyQueue, list<Process>& waitQueue)
{         
   // get list of children
   list<Process*> children = proc.GetChildren();
   
   // remove children from queues
   for (auto i = children.begin(); i != children.end(); i++)
   {
      TerminateChildren(outputFile, **i, readyQueue, waitQueue);       // terminate children of child
      
      outputFile << "PID " << (*i)->GetPID() << " " << (*i)->GetInitBurstTime() << " terminated\n"; // display message
      
      // remove from Ready Queue
      auto j = readyQueue.begin();
      while(j != readyQueue.end())
      {
	 if (j->GetPID() == (*i)->GetPID())
	    readyQueue.erase(j++);
	 else
	    j++;
      }
      
      // remove from Wait Queue
      j = waitQueue.begin();
      while(j != waitQueue.end())
      {
	 if (j->GetPID() == (*i)->GetPID())
	    waitQueue.erase(j++);
	 else
	    j++;
      }
   }
}
