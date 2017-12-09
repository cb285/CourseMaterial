#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define PORT 61501

int main(int argc, char* argv[])
{
   int socketD;

   // create tcp socket
   socketD = socket(AF_INET, SOCK_STREAM,0);
   cout << "socket descriptor = " << socketD << endl;

   // setup port and address to listen on
   struct sockaddr_in self;
   self.sin_family = AF_INET;
   self.sin_port = htons(PORT); // changes from host to network byte order
   self.sin_addr.s_addr = INADDR_ANY;

   // bind socket to port and address
   bind(socketD, (struct sockaddr*)&self, sizeof(self));

   cout << "server listening on port: " << PORT << endl;

   // wait for connection
   listen(socketD, 20);

   // create client address struct
   struct sockaddr_in client_addr;
   int addrlen = sizeof(client_addr);
   
   // accept client connection, creating new socket
   int clientfd = accept(socketD, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);

   // print client ip and port
   cout << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;
   
   while(1)
   {
      bool endofline = false;
      int recieve = 0;
      int iter = 0;
      char rebuffer[80];
      char buffer[80];
      
      while(!endofline)
      {
	 // receive message from client
	 recieve = recv(clientfd, rebuffer, 80, 0);
	 
	 for(int i = 0; i < recieve; i++, iter++)
	 {
	    if(rebuffer[i] == '\n')
	       endofline = true;
	    
	    buffer[iter] = rebuffer[i];
	    
	    if(endofline)
	       buffer[iter+1] = 0;
	 }
      }

      // send same message back to client
      send(clientfd, buffer, iter+1, 0);
   }
   
   close(clientfd);
   close(socketD);
   return 0;
}
