#ifndef _CS536_PA01_SERVER_H_
#define _CS536_PA01_SERVER_H_



#include <thread>


class SocketPool;
// Server class
class Server {
  public:
    int s_socketFD;
    bool stop;
    int port;

    // Constructor 
    Server(SocketPool *socket_pool_p, int portno);

    // stop the server;
    void Stop();

    // Begin to listen the port and loop forever to accept new clients
    void svlisten();

    std::thread *server_thread;
    SocketPool *socket_pool;    
};

#endif  // _CS536_PA01_SERVER_H_
