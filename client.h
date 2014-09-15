#ifndef _CS536_PA01_CLIENT_H_
#define _CS536_PA01_CLIENT_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h> 
#include <iostream>

#include <errno.h>

// The client class of rfss, each instance contains the information of the other
// end of the socked and the socked descriptor.
class Client {
  public:
    //std::string server_name;
    struct hostent *server;
    int server_port;
    struct sockaddr_in client_add;
    int c_socketFD;

    // Initialize the client to the server;
    void init(char *, int);

    // Upload the file to the server;
    void upload();

    // Stop the connection to the server;
    void stop();
};

#endif  // _CS536_PA01_CLIENT_H_
