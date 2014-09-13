#include <iostream>
#include <thread>
#include <netdb.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>

#include "rsff.h"
#include "client.h"
#include "server.h"

Rfss::Rfss(int port) {
  this.server.init(port);
}

Rfss::init() {
  // Let the server begin to listen
  std::thread server_thread(server.listen());
  // Take control of terminal
  
}

// Connected to a host;
Rfss::connect(string host_name, int port) {
  Client *client = new Client(host_name, port);
  if (client->connect()) {
    this.client_vec.append(client);
    std::cout << "Connect success";
  } else {
    std::cout << "Connect failed";
  }
}

int main() {
  int port = 12345;
  Rfss rfss(port);
  rfss.init(); 
}
