#include <cstring>
#include <iostream>
#include <string>
#include <cstdio>
#include <string.h>
// headers for socket(), getaddrinfo() and friends
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "dbms.hh"
#include <unistd.h>
#include "server.hh"
//database
using namespace dbms;
using namespace std;

int main(){
  //sets up and initialises the database
  dataBase *db;
  db=new dataBase();
  db->loadCSV("scifi.csv");

  sockaddr_in serv_addr, client_addr;
  char buffer[DEFAULT_STRLEN];

  cerr << "Starting server..." << std::endl;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(SERVER_PORT);

  //creates a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    cerr << "Error while creating socket" << std::endl;
    exit(1);
  }

  //binds the socket to an address
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    cerr << "Error while binding socket" << std::endl;
    close(sockfd);
    exit(1);
  }

  Parent://goto for forking
  //listens for connections with clients
  if (listen(sockfd, SOMAXCONN) < 0) {
    cerr << "Error while Listening on socket" << std::endl;
    close(sockfd);
    exit(1);
  }

  // structure large enough to hold client's address
  socklen_t client_addr_size = sizeof(client_addr);

  //accepting connection with client
  int clientfd = accept(sockfd, (sockaddr *) &client_addr, &client_addr_size);
  if (clientfd < 0) {
    cerr << "Error while Accepting on socket" << std::endl;
    exit(1);
  }

  //fork parent from child incase of more than one client
  pid_t p = fork();
  if(p>0)
  goto Parent;

  //Send hello to client -connection message
  int s = send(clientfd,"HELLO",6, 0);

  if (s < 0) {
    cerr << "Error while writing to socket" << std::endl;
    exit(1);
  }

  while (1) {
    //resets the buffer
    memset(buffer, 0, DEFAULT_STRLEN);
    //recieves message from client
    int r = recv(clientfd,buffer,DEFAULT_STRLEN, 0);
    if (r < 0) {
      cerr << "Error while reading from socket" << std::endl;
      exit(1);
    }
    cout << "Message received: " << buffer << std::endl;

    //check for closing connection
    if ((strcmp(buffer, "BYE") == 0) || (strcmp(buffer, "BYE\n") == 0) ){
      cout << "Closing connection with client." << std::endl;
      close(clientfd);
      close(sockfd);
      exit(0);
    }
    string out; //message to send to client
    //returns all of the database
    if ((strcmp(buffer, "GET") == 0) || (strcmp(buffer, "GET\n") == 0)){
      int index=0;
      out=db->getMovieToString(0, true);

    }else if(buffer[0]== 'G' && buffer[1]=='E' && buffer[2]=='T'){ //To get a specific row from the db
      //Getting index of movie that the message from the client contains
      int nullChar;
      char snum[20];
      for(nullChar=0; buffer[nullChar]!='\0'; nullChar++);
      memcpy(snum, &buffer[4], nullChar-4);
      snum[nullChar-4]='\0';
      int row=atoi(snum);
      //checking for valid input
      if(row < db->rowsUsed && row>=0 && !(row==0&&snum[0]!='0')){
        out= db->getMovieToString(row, false);
      }else{
        out="ERROR\n";
      }
    }else{
      out="ERROR\n";
    }

    //copys data to the buffer
    memset(buffer, 0, DEFAULT_STRLEN);
    strcpy(buffer, out.c_str());

    //sends data to client
    s = send(clientfd,buffer,strlen(buffer), 0);
    if (s < 0) {
      cerr << "Error while writing to socket" << std::endl;
      exit(1);
    }
  }
  return 0;
}
