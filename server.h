#ifndef _CS536_PA01_SERVER_H_
#define _CS536_PA01_SERVER_H_


#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include <strings.h>
#include <iostream>
#include <thread>
//#include <netdb.h>
//#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>

//#include "rfss.h"
//#include "client.h"

// Each worker corresponds to one client connection
class ServerWorker {
  public:
    int client_socketFD;

    ServerWorker(int csFD);
    void swread();


};

// Server class
class Server {
  public:
    int s_socketFD;
    bool stop;
    int port;

    Server(): stop(false) {};
    std::vector<ServerWorker*> server_worker_vec;
    std::vector<std::thread *> server_worker_thread_vec;

    // init the serve;
    void init(int port);

    // Begin to listen the port and loop forever to accept new clients
    void svlisten();

};

#endif  // _CS536_PA01_SERVER_H_
