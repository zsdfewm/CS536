#ifndef _SOCKET_POOL_H_
#define _SOCKET_POOL_H_


#include <pthread.h>
#include <vector>

using namespace std;
class Client;

class SocketPool{
 public:
  SocketPool();
 

  bool AddClient(int socketFD);
  
  void PrintList();

  void Stop();

  vector<Client*> client_pool;
  pthread_mutex_t pool_mutex;
};

#endif // _SOCKET_POOL_H_
