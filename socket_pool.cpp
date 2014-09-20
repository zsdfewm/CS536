#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <vector>
#include <thread>
#include <unistd.h>
#include "socket_pool.h"
#include "client.h"

SocketPool::SocketPool() {
  pool_mutex = PTHREAD_MUTEX_INITIALIZER;
  stop = false;
}

void SocketPool::Stop() {
  stop = true;
  pthread_mutex_lock(&pool_mutex);
  for(Client *client : client_pool) {
    client->Stop();
    client->client_thread->join();
  }
  pthread_mutex_unlock(&pool_mutex);
  daemon_thread->join();
}


bool SocketPool::DupCheck(const string& peer_name) {
  pthread_mutex_lock(&pool_mutex);
  for(Client *client : client_pool) {
    if (client->peer_name.compare(peer_name) == 0 ) {
       pthread_mutex_unlock(&pool_mutex);
      return true;
    }
  }
  pthread_mutex_unlock(&pool_mutex);
  return false;
}
// Returns true when the client is successfully add and the thread is running.
bool SocketPool::AddClient(int clisocketFD) {
  if (client_pool.size() >= SOCKET_POOL_SIZE) {
    cout << "socket pool full, reject the connection" << endl;
    return false;
  }
  pthread_mutex_lock(&pool_mutex);
  Client *client = new Client(clisocketFD);
  client_pool.push_back(client);
  cout << "New connection with : ";
  client->PrintInfo();
  client->client_thread = new thread(&Client::Run, client);
  pthread_mutex_unlock(&pool_mutex);
  return true;
}

void SocketPool::PrintList() {
  int counter = 0;
  for (Client *client : client_pool) {
    cout << ++counter << " : ";
    client->PrintInfo();
  }
}


void SocketPool::RunDaemonThread() {
  daemon_thread = new thread(&SocketPool::DaemonThread, this);
}

// A daemen thread which removes dead clients
void SocketPool::DaemonThread() {
  while(stop == false) {
    pthread_mutex_lock(&pool_mutex);
//    printf("Checking channel status...\n");
    vector<Client*>::iterator iter = client_pool.begin();
    Client* client;
    while(iter != client_pool.end()) {
      client = *iter;
      if (client->dead) {
        iter = client_pool.erase(iter);
        cout << "remove connection with: " << client->peer_name << endl;
        delete client;
      } else {
        ++iter;
      }
    }
//    printf("Checking channel status...done\n");
    pthread_mutex_unlock(&pool_mutex);
    usleep(1000);
  }
}
