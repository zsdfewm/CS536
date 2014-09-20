#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <vector>
#include <thread>

#include "socket_pool.h"
#include "client.h"

SocketPool::SocketPool() {
  pool_mutex = PTHREAD_MUTEX_INITIALIZER;
}

void SocketPool::Stop() {
  for(Client *client : client_pool) {
    client->Stop();
    client->client_thread->join();
  }
}


// Returns true when the client is successfully
// Then the caller will initialize the thread
bool SocketPool::AddClient(int clisocketFD) {
  pthread_mutex_lock(&pool_mutex);
  Client *client = new Client(clisocketFD);
  client_pool.push_back(client);
  client->client_thread = new thread(&Client::Run, client);
  pthread_mutex_unlock(&pool_mutex);
  return true;
}

void SocketPool::PrintList() {
  for (Client *client : client_pool) {
    client->PrintInfo();
  }
}
