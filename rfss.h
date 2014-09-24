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

    // Initialize the server listening
    void Init(char *, int);

    // Connect to a host
    void Connect(const string&, int port);

    // Show hostname and IP.
    void GetMyIp();

    // Show current connections
    void ShowList();

    // Terminate a given connection
    void Terminate(int);
    
    // Upload a file to a connection
    void Upload(int dest, const string& filename);

    // Generate file
    void GenerateFile(const string& filename, int file_size);

    // Stop the application
    void Stop();  
};



#endif  // _CS_536_PA01_RFSS_H_

