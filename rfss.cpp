#include <iostream>
#include <thread>
#include <netdb.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "rfss.h"
#include "client.h"
#include "server.h"
#include "socket_pool.h"

using namespace std;

void Rfss::Init(char * hostname, int port) {
    //client.init(hostname, port);
    host_name = hostname;
    socket_pool = new SocketPool();
    cout << "tryint server on: "<< port << endl;
    server = new Server(socket_pool, port);	//serverpart
    // Let the server begin to listen
    s_thread = new thread(&Server::svlisten, server);	//serverpart
    // Give the server 2 second to prepare
    sleep(2);
    socket_pool->RunDaemonThread();
}

// Connected to a host;
// When connect for the second time, error raises!!!
void Rfss::Connect(const string& hostname_str, int port) {

   if (host_name.compare(hostname_str) == 0 ) {
     cout << "Can not connect with self." << endl;
     return;
   }

   if (socket_pool->DupCheck(hostname_str)) {
     cout << "Already connected with: " << hostname_str << endl;
     return;
   }
   cout << "socket_pool_size" << SOCKET_POOL_SIZE << endl;
   if (socket_pool->client_pool.size() >= SOCKET_POOL_SIZE) {
     cout << "socket pool full, no connection created." << endl;
     return;
   }
   char *hostname = new char[hostname_str.size()+1];
   memcpy(hostname, hostname_str.c_str(), hostname_str.size());
   hostname[hostname_str.size()] = '\0';
   int socketFD = Client::Connect(hostname, port);
   delete[] hostname;
   if (!socket_pool->AddClient(socketFD)) {
     cout << "socket pool full, close the connection." << endl;
     close(socketFD);
   }
   return;
}

/* terminate one connection with host */
void Rfss::Terminate(int dest) {
  if (dest <= 0 || dest >  socket_pool->client_pool.size() ) {
    printf("Connectiong ID error: %d in [1, %d]\n", dest, socket_pool->client_pool.size());
  }
  socket_pool->client_pool[dest-1]->Stop();
  socket_pool->client_pool[dest-1]->client_thread->join();
  socket_pool->client_pool.erase(socket_pool->client_pool.begin()+dest-1);
}

/* terminate all connections and exit the process */
void Rfss::Stop(){
    cout<<"Shutdown after 2 seconds" << endl;
    sleep(2);
    server->Stop();
    s_thread->join();
    socket_pool->Stop();
}

/* display the ip of local machine */
void Rfss::GetMyIp() {
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    //cout<<"Testing: AF_INET=" << AF_INET <<endl;

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
	//cout << "name:" << ifa->ifa_name<< "\tsa_family: " << ifa->ifa_addr->sa_family <<endl;

        if (!(ifa->ifa_addr)) {
            continue;
        }

        else if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address

	    //cout << ifa->ifa_name << " equal to AF_INET" << endl;

            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
	    //cout << "Test ..." << tmpAddrPtr <<endl;

            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("IPv4 %s IP Address %s\n", ifa->ifa_name, addressBuffer);
	    //freeifaddrs(ifAddrStruct);
	    //return true;
        } 
    }
    if (ifAddrStruct!=NULL) 
    {
	freeifaddrs(ifAddrStruct);
	ifAddrStruct = NULL;
    }
    //return false;
}


void Rfss::ShowList() {
  socket_pool->PrintList();
}

void Rfss::Upload(int dest, const string& filename) {
  if (dest <= 0 || dest >  socket_pool->client_pool.size() ) {
    printf("Connectiong ID error: %d in [1, %d]\n", dest, socket_pool->client_pool.size());
  }
  socket_pool->client_pool[dest-1]->SendFile(filename);

}
void Rfss::GenerateFile(const string& file_name, int file_size) {
  int result = mkdir("./upload", 0777);
  string filename = "./upload/" + file_name;
  char *c_filename = new char[filename.size()+1];
  memcpy(c_filename, filename.c_str(), filename.size());
  c_filename[filename.size()] = '\0';
  file_size *= 1024;
  printf("generateing %s, size = %d\n", c_filename, file_size); 
  FILE *fp = fopen(filename.c_str(), "w");
  if (fp != NULL) {
    printf("OK!\n");
  } else {
    printf("ERROR!\n");
  }
  fflush(stdout);
  char c;
  for(int i = 0; i< file_size; ++i) {
    c = 48 + (i % 10);
    fputc(c, fp);
//    printf("%c", c);
  }
  fclose(fp);
}
