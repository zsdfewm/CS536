#ifndef _CS536_PA01_RFSS_H_
#define _CS536_PA01_RFSS_H_

class Client;
class Server;
// The main class of the remote file sharing system
class Rfss {
 public:
  // The constructor
  Rfss(int port);

  // init the TCP server, and take control of the terminal
  void init();

  // connect to a host
  void connect(string host_name, int port);  
  
  std::Vector<Client*> client_vec;
  Server server;
}



#endif  // _CS_536_PA01_RFSS_H_

