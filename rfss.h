#ifndef _CS536_PA01_RFSS_H_
#define _CS536_PA01_RFSS_H_

#include <vector>
#include <thread>

using namespace std;

class Client;
class Server;
class SocketPool;

// The main class of the remote file sharing system
class Rfss {
  public:
    
    SocketPool *socket_pool;
    Server *server;

    thread *s_thread;
    // The constructor
    //Rfss(char *, int);

    // init the TCP server, and take control of the terminal
    void init(char *, int);

    // connect to a host
    void connect(char *, int port);

    void GetMyIp();

    void ShowList();

    //terminate a given connection
    void terminate(int);

    void GenerateFile(char* file_name, int file_size);

    //exit the application
    void Stop();
  
};



#endif  // _CS_536_PA01_RFSS_H_

