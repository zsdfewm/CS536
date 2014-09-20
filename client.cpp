#include <netinet/in.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <iostream>
#include <errno.h>
#include <stdint.h>
#include <limits>
#include <fcntl.h>

#include"client.h"

using namespace std;

Client::Client(int socketFD_p) {
  socketFD = socketFD_p;
  stop = false;
  fcntl(socketFD, F_SETFL, O_NONBLOCK);
}

Client::~Client() {
}

void Client::Stop() {
  stop = true;
}

void Client::Run() {
  char buff[1000];
  int len;
  while(stop == false) {
    len = read(socketFD, buff, 1000);
    if (len == -1) {
      if (errno != EWOULDBLOCK) {
        cout << "ERROR on read" << endl;
      }
    } else {
      if (len != 0) {
        buff[len]=0;
        printf("Got message: %s\n", buff);
      }
    }
  }
  close(socketFD);
}

bool Client::Send(char *data, int len) {
  int write_len = write(socketFD, data, len);
  if (write_len < 0) {
    cout << "ERROR on write" << endl;
    return false;
  } else if (write_len < len) {
    printf("Non complete write %d/%d \n", write_len, len);
  }
  return true;
}

void Client::PrintInfo() {
  cout << "dummy info" << endl;
}


int Client::Connect(char* hostname, int portno) {
  struct hostent* server = gethostbyname(hostname);
  if (server == NULL) {
    printf("ERROR, no such host: %s \n", hostname);
  }
  int port = portno;
  struct sockaddr_in serv_addr;
  string buffer("Hello!!! The connection is established!");

  int c_socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (c_socketFD < 0) 
      cout << "ERROR opening socket" <<endl;

  bzero((char *) &serv_addr, sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
       (char *)&serv_addr.sin_addr.s_addr,
       server->h_length);
  serv_addr.sin_port = htons(port);
  printf("trying to connect %s @ %d\n", hostname, port);
  if (connect(c_socketFD,
              (struct sockaddr *) &serv_addr,
              sizeof(struct sockaddr_in)) < 0) {
    cout << "ERROR connecting" << endl << strerror(errno) <<endl;
  }
  char buff[100] = "hello channel\n";
  int len = write(c_socketFD, buff, 20);
  if (len < 0) {
    cout << "ERROR writing to socket" <<endl;
  }
  return c_socketFD;
}

/*
//std::numeric_limits<int32_t>::max();

// Initialize the client to the server;
void Client::init(char * hostname, int portno)
{
    cout << "Test... hostname" << hostname <<endl;
    
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    server_port = portno;
}

// Upload the file to the server;
void Client::ConnectServer()
{
    struct sockaddr_in serv_addr;
    //char buffer[512];
    //string buffer;
    //bzero(buffer,100); 
    string buffer("Hello!!! The connection is established!");
    //str.copy(buffer, 199, 0);
    int len;

    c_socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (c_socketFD < 0) 
        cout << "ERROR opening socket" <<endl;
    //server = 
    
    bzero((char *) &serv_addr, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(server_port);
    if (connect(c_socketFD,(struct sockaddr *) &serv_addr,sizeof(struct sockaddr_in)) < 0) 
        cout << "ERROR connecting" << endl << strerror(errno) <<endl;
    //cout << "Please enter the message: " <<endl;  

    //cin >> buffer;
    //cout << "Testing... " << buffer <<endl;
    char *buff = new char[100];
    buff[buffer.size()]=0;
    memcpy(buff, buffer.c_str(), buffer.size());


    len = write(c_socketFD,buff,buffer.size()+1);
    if (len < 0) 
         cout << "ERROR writing to socket" <<endl;
    bzero(buff,100);
    len = read(c_socketFD,buff,99);
    if (len < 0) 
         cout << "ERROR reading from socket" <<endl;
    cout << "The message is: " << buffer <<endl;
}


// Stop the connection to the server;
void Client::stop()
{
    close(c_socketFD);
}

*/
