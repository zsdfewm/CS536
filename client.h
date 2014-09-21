#ifndef _CS536_PA01_CLIENT_H_
#define _CS536_PA01_CLIENT_H_

#include <string>
#include <thread>
#include <iostream>
#include <fstream>

/*
class Message{
    char msgname[256];
    int msglen;
};
*/
//using namespace std;

// The client class of rfss, each instance contains the information of the other
// end of the socked and the socked descriptor.
class Client {
  public:
    Client(int socketFD);
    //~Client();

    void Run();
    bool Send(char *data, size_t len);
    void Stop();

    bool SendFile(const std::string& filename_str);
    void PrintInfo();

    static int Connect(char* hostname, int portno);

    bool stop;
    bool dead;
    std::thread *client_thread;
    std::string host_name;
    std::string peer_name;
    std::string peer_ip;
//    struct hostent *peer_host;
//    struct sockaddr_in peer_add;
    int socketFD;
    static int PACKAGE_SIZE;
};

#endif  // _CS536_PA01_CLIENT_H_
