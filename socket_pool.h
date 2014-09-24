#ifndef _SOCKET_POOL_H_
#define _SOCKET_POOL_H_


#include <pthread.h>
#include <vector>
#include <thread>

#define SOCKET_POOL_SIZE 5

class Client;

// The socketPool, client_pool contains all client instances of 
// current connections. A daemon thread will loop over the instances
// to collect dead connections.
class SocketPool{
 public:
  SocketPool();
 
  bool AddClient(int socketFD);
  
  void PrintList();

  void Stop();

  void DaemonThread();

  void RunDaemonThread();
  
  bool DupCheck(const std::string& peer_name);

  bool stop;

  std::vector<Client*> client_pool;
  pthread_mutex_t pool_mutex;
  std::thread *daemon_thread;
};

#endif // _SOCKET_POOL_H_
