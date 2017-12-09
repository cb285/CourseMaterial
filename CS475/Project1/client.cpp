#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h> // gethostbyname
#include <string.h> // memset, memcpy

using namespace std;

int main(int argc, char* argv[])
{
   if (argc != 3)
   {
      cout << "USAGE: " << argv[0] << " hostname port" << endl;
      return 0;
   }

   // get arguments
   string hostname = argv[1];
   int port = atoi(argv[2]);

   // create tcp socket
   int socketD;
   socketD = socket(AF_INET, SOCK_STREAM,0);

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
   
   string message;
   
   char response[80];

   while(true)
   {
      // get message to send
      getline(cin, message);
      message.append("\n");

      // send message to server
      send(socketD,message.c_str(),message.length(), 0);
      
      // get response from server
      recv(socketD, response, 80, 0);
      
      // print responce
      cout << response;
   }
   
   close(socketD);
   return 0;
}
