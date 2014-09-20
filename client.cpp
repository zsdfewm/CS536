#include <netinet/in.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <iostream>
#include <errno.h>
#include <stdint.h>
#include <limits>
#include <fcntl.h>

#include"client.h"

#define STATUS_IDLE 0
#define STATUS_BUSY 1

#define PACKAGE_SIZE 512

using namespace std;

char kMagicNumber1[] = "31415926";
char kMagicNumber2[] = "27182818";
size_t MAGIC_SIZE = 8;


Client::Client(int socketFD_p) {
  socketFD = socketFD_p;
  stop = false;
  dead = false;
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
  FILE *fp;
  int status = STATUS_IDLE;
  int filesize;
  int written_size;
  char filename[1000];

  printf("socket %x running\n", socketFD);
  while(stop == false) {
//    printf("trying reading socket %x \n", socketFD);
    len = read(socketFD, buff, PACKAGE_SIZE);
    if (len == -1) {
//      printf("errno = %x\n", errno);
      if (errno != EWOULDBLOCK) {
        cout << "ERROR on read, stop socket" << endl;
        break;
      }
    } else {
      if (len != 0) {
        printf("message len = %d\n", len);
        if (status == STATUS_IDLE) {
          buff[len]=0;
          printf("Got message: %s\n", buff);
          bool check = true;
          if (len < MAGIC_SIZE+sizeof(int)+1) {continue;}
          for(size_t i = 0; i < MAGIC_SIZE; ++i) {
            if (buff[i] != kMagicNumber1[i]) {
              check = false;
            }
          }
          if (!check) {continue;}
          filesize =* (int*)(&buff[8]);
          size_t l = MAGIC_SIZE+sizeof(int);
          size_t i = 0;
          for(i = 0; l < len; ++i, ++l) {
            filename[i]=buff[l];
          }
          filename[i] = 0;
          status = STATUS_BUSY;
          printf("waiting for file %s, size=%d\n", filename, filesize);
          int result = mkdir("./upload", 0777);
          fp = fopen(filename, "w");
          written_size = 0;
        } else if (status == STATUS_BUSY) {
          if (written_size < filesize) {
            if (len != fwrite(buff, sizeof(char), len, fp)) {
              printf("Local file written error");
            } else {
              written_size += len;
            }
          } else {
            bool check = false;
            if (len == MAGIC_SIZE) {
              check = true;
              for(size_t i = 0; i < MAGIC_SIZE; ++i) {
                if (buff[i] != kMagicNumber2[i]) {check = false;}
              }
            }
            if (check) {
              printf("file transfer done!\n");
              status = STATUS_IDLE;
            } else {
              printf("Got a message(X): %s\n", buff);
            }
          }
        }
      } else {
//        printf("len == 0 \n");
        cout << "Length 0 message (closed channel?)" << endl;
        break;
      }
    }
  }
  printf("socket %x closed\n", socketFD);
  shutdown(socketFD, 2);
  dead = true;
}

bool Client::Send(char *data, size_t len) {
  size_t write_len = write(socketFD, data, len);
  if (write_len < 0) {
    cout << "ERROR on write" << endl;
    return false;
  } else if (write_len < len) {
    printf("Non complete write %d/%d \n", write_len, len);
  }
  return true;
}

void padding(char* buff, int used_len) {
  for(int l = used_len; l < PACKAGE_SIZE; ++l) {
    buff[l]=0;
  }
}

bool Client::SendFile(const string& file_name) {
  int result = mkdir("./upload", 0777);
  string filename = "./upload/" + file_name;
  char* c_filename = new char[filename.size()+1];
  memcpy(c_filename, filename.c_str(), filename.size());
  c_filename[filename.size()] = 0;
  int filesize;
  FILE *fp = fopen(c_filename, "r");
  fseek(fp, 0, SEEK_END);
  filesize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  printf("reading file %s, size = %d\n", c_filename, filesize);
  char buff[PACKAGE_SIZE];

// 1: send magic number#1+filesize+filename
  size_t l=0;
  size_t i=0;
  while(i<MAGIC_SIZE) {
    buff[l++] = kMagicNumber1[i++];
  }
  char *tmp=(char*) (&filesize);
  i=0;
  while(i<sizeof(int)) {
    buff[l++]=tmp[i++];
  }
  i=0;
  while(i<filename.size()) {
    buff[l++]=filename.c_str()[i++];
  }
  buff[l]=0;
  for(i=0;i<l;i++){
   printf("%c", buff[i]);
  }
  printf("\n");
  padding(buff, l);
  if (!Send(buff, PACKAGE_SIZE)) {
    printf("Sending MagicNumber 1 error.\n");
    fclose(fp);
    return false;
  }
// 2: send file contents
  int read_len;
  while ((read_len = fread(buff, sizeof(char), PACKAGE_SIZE, fp)) > 0) {
    Send(buff, read_len);
  }
// 3: send magic number#2
  size_t l=0;
  size_t i=0;
  while( i < MAGIC_SIZE ) {
    buff[l++] = kMagicNumber2[i++];
  }
  padding(buff, l);
  Send(buff, PACKAGE_SIZE);
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
