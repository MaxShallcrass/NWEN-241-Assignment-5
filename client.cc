#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>

//#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>


#include "server.hh"

int main() {

  sockaddr_in serv_addr;
  char buffer[DEFAULT_STRLEN];

  std::cerr << "Starting client..." << std::endl;

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;  // this should be the IP
  // address of the server
  serv_addr.sin_port = htons(SERVER_PORT);

  //creates a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "Error while creating socket" << std::endl;
    exit(1);
  }

  //connects socket to server address
  if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    close(sockfd);
    std::cerr << "Error while connecting socket" << std::endl;
    exit(1);
  }

  //Gets connection message
  int bytes_recv = recv(sockfd, buffer, DEFAULT_STRLEN, 0);
  if (bytes_recv < 0) {
    std::cerr << "Error while receiving bytes" << std::endl;
    exit(1);
  } else if (bytes_recv == 0) {
    close(sockfd);
    std::cout << "Closing socket." << std::endl;
    exit(1);
  }

  std::cout << "\nServer connection message:\n" << buffer << std::endl;
  std::cout << "Comands: 'GET' and 'GET (int)'" <<  std::endl;

  while (1){
    memset(buffer, 0, DEFAULT_STRLEN);
    std::cout << "Please enter the command: ";
    std::cin.getline(buffer, DEFAULT_STRLEN);
    //send Message
    int bytes_sent = send(sockfd, buffer, strlen(buffer), 0);
    if (bytes_sent < 0) {
      std::cerr << "Error while sending bytes" << std::endl;
      exit(1);
    }
    //Exit comment made
    if ((strcmp(buffer, "BYE") == 0) || (strcmp(buffer, "BYE\n") == 0) ){
      std::cerr << "Closing connection with server." << std::endl;
      exit(1);
    }

    memset(buffer, 0, DEFAULT_STRLEN);
    //message recieved
    bytes_recv = recv(sockfd, buffer, DEFAULT_STRLEN, 0);
    if (bytes_recv < 0) {
      std::cerr << "Error while receiving bytes" << std::endl;
      exit(1);
    } else if (bytes_recv == 0) {
      close(sockfd);
      std::cout << "Closing socket." << std::endl;
      exit(1);
    }
    std::cout << "\nClient recieved:\n" << buffer << std::endl;
  }
  return 0;
}
