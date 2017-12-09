/* Clayton Brutus
   CS475 Networks
   Project 2
   client.cpp
 */

#include <iostream>     // cin, cout
#include <sys/socket.h> // sockets
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h> // gethostbyname
#include <string.h> // memset, memcpy
#include <stdlib.h> // sleep

using namespace std;

int main(int argc, char* argv[])
{
   // check for correct number of arguments
   if (argc != 3)
   {
      cout << "USAGE: " << argv[0] << " hostname port" << endl;
      return 0;
   }
   
   // get hostname and port from arguments
   string hostname = argv[1];
   int port = atoi(argv[2]);
   
   string message;
   
   char response[100];
   
   // loop until user chooses to quit
   while(true)
   {
      // create tcp socket
      int socketD = socket(AF_INET, SOCK_STREAM,0);
      
      // get server ip from hostname
      struct hostent *hostEntry;
      hostEntry = gethostbyname(hostname.c_str());
      
      // setup server address
      struct sockaddr_in server_addr;
      memset(&server_addr,0,sizeof(server_addr));
      server_addr.sin_family = AF_INET;
      server_addr.sin_port = htons(port); // changes from host to network byte order
      memcpy(&server_addr.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);
      
      // connect to server
      connect(socketD, (struct sockaddr *)&server_addr, sizeof(server_addr));
      
      // get menu from server
      int r = recv(socketD, response, 100, 0);
      response[r] = '\0';
      
      // print menu
      cout << response << endl;

      // get city choice from user
      getline(cin, message);	 
      message.append("\0");
	 
      // send choice to server
      send(socketD, message.c_str(), message.length(), 0);
      
      // recieve and print weather data
      while((r = recv(socketD, response, 100, 0)) > 0)
      {
	 response[r] = '\0';
	 cout << response;
	 sleep(.2);
      }
      
      // close connection
      close(socketD);

      // ask if user wants to get another city's weather
      cout << "get weather for another city? (y/n) ";
      
      string choice;
      cin >> choice;
      
      response[0] = '\0';
      message = "";
      
      // if they don't then break from loop
      if (choice != "y")
	 break;

      // ignore newline character in stream
      cin.ignore();
   }

   // exit
   return 0;
}
