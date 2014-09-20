#ifndef _CS536_PA01_RFSS_H_
#define _CS536_PA01_RFSS_H_

#include <vector>
#include <string>
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
    string host_name;
    thread *s_thread;
    // The constructor
    //Rfss(char *, int);

    // init the TCP server, and take control of the terminal
    void Init(char *, int);

    // connect to a host
    void Connect(const string&, int port);

    void GetMyIp();

    void ShowList();

    //terminate a given connection
    void Terminate(int);
    void Upload(int dest, const string& filename);
    void GenerateFile(const string& filename, int file_size);

    //exit the application
    void Stop();  
};



#endif  // _CS_536_PA01_RFSS_H_

