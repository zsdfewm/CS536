#ifndef _CS536_PA01_SERVER_H_
#define _CS536_PA01_SERVER_H_


//#include "rfss.h"
//#include "client.h"

#include <thread>

using namespace std;
// Each worker corresponds to one client connection

class SocketPool;

/*
class ServerWorker {
  public:
    int client_socketFD;

    ServerWorker(int csFD);
    void swread();
    void stop();


};
*/

// Server class
class Server {
  public:
    int s_socketFD;
    bool stop;
    int port;

    // Constructor 
    Server(SocketPool *socket_pool_p, int portno);
//    std::vector<ServerWorker*> server_worker_vec;
//    std::vector<std::thread *> server_worker_thread_vec;
//    std::vector<struct sockaddr *> cli_addr_vec;

    // stop the server;
    void Stop();

    // Begin to listen the port and loop forever to accept new clients
    void svlisten();

    thread *server_thread;
    SocketPool *socket_pool;    
};

#endif  // _CS536_PA01_SERVER_H_
