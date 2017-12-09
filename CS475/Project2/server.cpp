/* Clayton Brutus
   CS475 Networks
   Project 2
   server.cpp
 */

#include <iostream>     // cin, cout
#include <sys/socket.h> // sockets
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>  // gethostbyname
#include <string.h> // memset, memcpy

#include <string>   // string
#include <sstream>  // stringstream

#include <vector>   // vector

#include "rapidxml.hpp"  // XML parser
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace std;

// server listening port
#define LISTENPORT 61501

int main(int argc, char* argv[])
{
   // create tcp socket
   int socketD = socket(AF_INET, SOCK_STREAM,0);
   
   // setup port and address to listen on
   struct sockaddr_in self;
   self.sin_family = AF_INET;
   self.sin_port = htons(LISTENPORT); // changes from host to network byte order
   self.sin_addr.s_addr = INADDR_ANY;
   
   // bind socket to port and address
   bind(socketD, (struct sockaddr*)&self, sizeof(self));
   
   // make socket listen for connections
   listen(socketD, 20);
   
   cout << "server listening on port: " << LISTENPORT << endl;
   
   // create client address struct
   struct sockaddr_in client_addr;
   int addrlen = sizeof(client_addr);

   // loop forever
   while(1)
   {
      // accept client connection, creating new socket
      int clientfd = accept(socketD, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);
      
      // print client ip and port
      cout << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;
      
      // send menu showing available cities
      string menu = "Select a city: [1] Evansville, IN   [2] Lafayette, IN   [3] Atlanta, GA\0";
      
      send(clientfd, menu.c_str(), menu.length(), 0);
      
      // get response from client
      char resp[120];
            
      // get choice
      recv(clientfd, resp, 120, 0);
      int citynum = atoi(resp);

      // get city from number
      string city;
      
      if(citynum == 1)
	 city = "IN/evansville";
      else if (citynum == 2)
	 city = "IN/lafayette";
      else
	 city = "GA/atlanta";
      
      // create socket for wunderground
      int weatherD = socket(AF_INET, SOCK_STREAM,0);
      
      // get server ip from hostname
      struct hostent *hostEntry;
      hostEntry = gethostbyname("api.wunderground.com");
      
      // setup server address
      struct sockaddr_in server_addr;
      memset(&server_addr,0,sizeof(server_addr));
      server_addr.sin_family = AF_INET;
      server_addr.sin_port = htons(80); // changes from host to network byte order
      memcpy(&server_addr.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);
       
      // connect to wunderground server
      connect(weatherD, (struct sockaddr *)&server_addr, sizeof(server_addr));

      // create GET request string
      stringstream ss;
      ss << "GET /api/cb09cf5b334aa6a9/forecast10day/conditions/q/" << city << ".xml HTTP/1.0\r\nHost: api.wunderground.com\r\n\r\n";
      string ssstr = ss.str();

      // send request to wunderground server
      send(weatherD, ssstr.c_str(), ssstr.length(), 0);
      
      string respstr;
      
      int r = 0;

      // recieve weather data from wunderground
      while((r = recv(weatherD, resp, 100, 0)) > 0)
      {
	 resp[r] = '\0';
	 respstr.append(resp);
      }

      // close wunderground socket
      close(weatherD);
      
      // remove everything before XML data
      size_t pos = respstr.find("<response>");
      respstr = respstr.substr(pos);      

      // parse with rapidXML
      rapidxml::xml_document<> doc;
      doc.parse<0>(&respstr[0]);
      
      // get current conditions
      string currentTemp = doc.first_node()->first_node("current_observation")->first_node("temperature_string")->value();
      string currentCond = doc.first_node()->first_node("current_observation")->first_node("weather")->value();
      
      // create string stream for putting weather data
      stringstream weatherresp;
      weatherresp << "CURRENT:" << endl;
      weatherresp << "  " << currentTemp << ", " << currentCond << endl << endl;;
      
      // get forecast for next 10 days
      weatherresp << "10 DAY FORECAST:" << endl;
      
      for (rapidxml::xml_node<>* n = doc.first_node()->first_node("forecast")->first_node("simpleforecast")->first_node("forecastdays")->first_node("forecastday"); n; n = n->next_sibling())
      {
	 string weekday =  n->first_node("date")->first_node("weekday")->value();
	 string month =  n->first_node("date")->first_node("monthname_short")->value();
	 string day =  n->first_node("date")->first_node("day")->value();
	 
	 string high_temp = n->first_node("high")->first_node("fahrenheit")->value();
	 string low_temp = n->first_node("low")->first_node("fahrenheit")->value();
	 string conditions = n->first_node("conditions")->value();
	 
	 weatherresp << "  " << weekday << ", " << month << " " << day << ":" << endl;
	 weatherresp << "    " << conditions << endl;
	 weatherresp << "    High: " << high_temp << " F" << endl;
	 weatherresp << "    Low: "<< low_temp << " F" << endl;
      }

      // convert string stream to string to send to client
      string weatherstr = weatherresp.str();
      weatherstr.append("\0");
      
      // send weather data to client
      send(clientfd, weatherstr.c_str(), weatherstr.length(), 0);
      
      // close client socket
      close(clientfd);
   }
   
   close(socketD);
   return 0;
}
