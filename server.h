#ifndef _CS536_PA01_SERVER_H_
#define _CS536_PA01_SERVER_H_


// Each worker corresponds to one client connection
class ServerWorker {
}

// Server class
class Server {
 public:
  // init the serve;
  void init(port);

  // Begin to listen the port and loop forever to accept new clients
  void listen();

  int serverFD;
  bool stop = false;
  std::Vector<ServerWorker> server_worker_vec;
}

#endif  // _CS536_PA01_SERVER_H_
