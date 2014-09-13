

//ServerWorker;

void ServerWorker::ServerWorker(int socketFD) {
  this.socketFD=socketFD;
}

void ServerWorker::read() {
  int len;
  while (len=read(socketFD()) != -1) {
    // deal with the reading
  }
}
void Server::init(int port) {
  this.port = port;
}

void Server::listen() {
  //init server socket
  //bind the server socket to the listen port
  //listen to the port

  while(stop == false) {
    // try to accept a client;
    client_socketFD = accept()
    if (client_socketFD != error) {
      ServerWorker *server_worker = new ServerWorker(client_sockekFD);
      std::thread *server_worker_thread
        = new thread(server_worker.read());
      server_worker_vec.push_back(server_worker);
      server_worker_thread_vec.push_back(server_worker_thread);
    }
  }
  // iterater over the vector and release the memory of server_wor
  
}
