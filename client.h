#ifndef _CS536_PA01_CLIENT_H_
#define _CS536_PA01_CLIENT_H_

#include "sys/socket.h"

// The client class of rfss, each instance contains the information of the other
// end of the socked and the socked descriptor.
class Client {
 public:
  // Initialize the client to the server;
  void init();

  // Upload the file to the server;
  void upload();

  string server_name;
  int server_port;
  struct sockedaddr_in client_add;
  int sockfd;
}

#endif  // _CS536_PA01_CLIENT_H_
