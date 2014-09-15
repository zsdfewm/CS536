#ifndef _CS536_PA01_RFSS_H_
#define _CS536_PA01_RFSS_H_

#include <iostream>
#include <thread>
#include <netdb.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>

//#include "rfss.h"
#include "client.h"
#include "server.h"

class Client;
class Server;

// The main class of the remote file sharing system
class Rfss {
  public:
    std::vector<Client*> client_vec;
    std::vector<std::thread *> server_thread;

    Server server;
    Client client;

    std::thread *s_thread;
    // The constructor
    Rfss(char *, int);

    // init the TCP server, and take control of the terminal
    void init();

    // connect to a host
    void connect(char *, int port);

    //terminate a given connection
    void terminate(int);

    //exit the application
    void exit();
  
};



#endif  // _CS_536_PA01_RFSS_H_

