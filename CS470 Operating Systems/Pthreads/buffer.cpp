#include <iostream>     // cout
#include <pthread.h>    // pthreads
#include <semaphore.h>  // semaphores
#include <unistd.h>     // sleep()
#include <time.h>       // time()

using namespace std;

typedef int buffer_item;
#define BUFFER_SIZE 10  // max number of items in buffer
#define MAX_ITEM 1000   // max item number producer can generate

void *Producer (void* param);
void *Consumer (void* param);
int insert_item(buffer_item item);
int remove_item(buffer_item* item);

buffer_item buffer[BUFFER_SIZE]; // the buffer
int sleeptime; // max sleep time

pthread_mutex_t mutex;
pthread_mutex_t outputmutex;
sem_t empty;
sem_t full;

// Function: main
// reads arguments, initializes variables
// creates threads
int main(int argc, char* argv[])
{
   // if 4 arguments not given, display message and exit
   if (argc != 4)
   {
      cout << "USAGE: " << argv[0] << " sleepTime numProducers numConsumers\n";
      return -1;
   }
   
   // vars to store arguments
   int sleep, numP, numC;
   
   // sleep time
   sleeptime = atoi(argv[1]);

   // number of Producers
   numP = atoi(argv[2]);
   
   // number of Consumers
   numC = atoi(argv[3]);
   
   // initialize semaphores
   sem_init (&empty, 0, BUFFER_SIZE); // empty
   sem_init (&full, 0, 0); // full

   // initialize buffer
   for (int i = 0; i < BUFFER_SIZE; i++)
      buffer[i] = -1;

   // seed random number generator
   srand(time(0));
   
   // create pthread arrays
   pthread_t Pt[100];
   pthread_t Ct[100];
   
   // create producer threads
   for (int i = 0; i < numP; i++)
   {      
      if (pthread_create(&Pt[i], NULL, Producer, (void *) (intptr_t) i))
      {
	 // display error and exit if failed
	 cout << "Error creating producer threads\n";
	 return -1;
      }
      
      else
	 cout << "Created Producer # " << i << "\n";
   }
   
   // create consumer threads
   for (int i = 0; i < numC; i++)
   {
      // create thread
      if (pthread_create(&Ct[i], NULL, Consumer, (void *) (intptr_t) i))
      {
	 // display error and exit if failed
	 cout << "Error creating consumer threads\n";
	 return -1;
      }

      else
	 cout << "Created Consumer # " << i << "\n";
   }
   
   // exit after all threads finish
   pthread_exit(NULL);
}

void *Producer (void* param)
{   
   buffer_item item;
   
   int pid = (intptr_t)param;

   while (true)
   {
      // sleep random time between 0 and given sleeptime
      sleep (rand() % (sleeptime + 1));
      
      // generate random number
      item = rand() % (MAX_ITEM + 1);

      // get output mutex lock
      pthread_mutex_lock(&outputmutex);
      
      cout << "Producer # " << pid << " waiting on empty\n";

      // release output mutex lock
      pthread_mutex_unlock(&outputmutex);
      
      // wait on empty semaphore
      sem_wait (&empty);
      
      // get output mutex lock
      pthread_mutex_lock(&outputmutex);
      
      cout << "Producer # " << pid << " getting mutex lock\n";

      // release output mutex lock
      pthread_mutex_unlock(&outputmutex);
      
      // get mutex lock
      pthread_mutex_lock(&mutex);

      // attempt to insert item
      if (insert_item(item))
      {
	 // if failed:
	 // get output mutex lock
	 pthread_mutex_lock(&outputmutex);

      	 // print error message
	 cout << "Error inserting item\n";
	 
	 // release output mutex lock
	 pthread_mutex_unlock(&outputmutex);
      }
      else
      {
	 // if succeeded display message

	 // get output mutex lock
	 pthread_mutex_lock(&outputmutex);
	 
	 cout << "Producer # " << pid << " produced " << item << "\n";
	 
	 cout << "Producer # " << pid << " signaling full\n";

	 // release output mutex lock
	 pthread_mutex_unlock(&outputmutex);
	 
	 sem_post(&full);
      }
      
      // get output mutex lock
      pthread_mutex_lock(&outputmutex);
      
      cout << "Producer # " << pid << " releasing mutex lock\n";
      
      // release output mutex lock
      pthread_mutex_unlock(&outputmutex);
      
      // release mutex lock
      pthread_mutex_unlock(&mutex);
   }
}

void *Consumer (void* param)
{   
   buffer_item item;
   int cid = (intptr_t)param;
   
   while (true)
   {
      // sleep random time
      sleep(rand() % sleeptime);
      
      // get output mutex lock
      pthread_mutex_lock(&outputmutex);
      
      cout << "Consumer # " << cid << " waiting on full\n";
      
      // release output mutex lock
      pthread_mutex_unlock(&outputmutex);
      
      // wait on full semaphore
      sem_wait (&full);
      
      // get output mutex lock
      pthread_mutex_lock(&outputmutex);
      
      cout << "Consumer # " << cid << " getting mutex lock\n";
      
      // release output mutex lock
      pthread_mutex_unlock(&outputmutex);
      
      // get mutex lock
      pthread_mutex_lock(&mutex);
      
      // attempt to remove item
      if (remove_item(&item))
      {
	 // if failed:
	 // get output mutex lock
	 pthread_mutex_lock(&outputmutex);
	 
	 // display message
	 cout << "Error removing item\n";

	 // release output mutex lock
	 pthread_mutex_unlock(&outputmutex);
      }	 
      else
      {
	 // if succeeded:
	 // get output mutex lock
	 pthread_mutex_lock(&outputmutex);
	 
	 cout << "Consumer # " << cid << " consumed " << item << "\n";

	 // release output mutex lock
	 pthread_mutex_unlock(&outputmutex);
      }
      
      // get output mutex lock
      pthread_mutex_lock(&outputmutex);
      
      cout << "Consumer # " << cid << " releasing mutex lock\n";

      // release output mutex lock
      pthread_mutex_unlock(&outputmutex);
      
      // release mutex lock
      pthread_mutex_unlock(&mutex);

      // get output mutex lock
      pthread_mutex_lock(&outputmutex);
      
      cout << "Consumer # " << cid << " signaling empty\n";

      // release output mutex lock
      pthread_mutex_unlock(&outputmutex);
      
      // signal empty semaphore
      sem_post(&empty);
   }
}

// Function: insert_item
// given an item, inserts it in first empty spot in buffer
// returns 0 if successful, -1 if failed
int insert_item(buffer_item item)
{
   // find first empty space
   int i = 0;
   
   while ((buffer[i] != -1) && (i < BUFFER_SIZE))
      i++;
   
   // return -1 if error (no empty spaces)
   if (i == BUFFER_SIZE)
      return -1;
   else
   {
      // insert item
      buffer[i] = item;
      
      // return 0 if successful
      return 0;
   }
}

// Function: remove_item
// finds first item and buffer, passes it back
// returns 0 if successful, -1 if failed
int remove_item(buffer_item *item)
{
  // find first non-empty space
   int i = 0;
   
   while ((buffer[i] == -1) && (i < BUFFER_SIZE))
      i++;
   
   // return -1 if error (buffer is empty)
   if (i == BUFFER_SIZE)
      return -1;
   else
   {
      // retreve item
      *item = buffer[i];
      
      // set space to empty
      buffer[i] = -1;
      
      // return 0 if successful
      return 0;
   }
}
