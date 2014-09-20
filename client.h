#ifndef _CS536_PA01_CLIENT_H_
#define _CS536_PA01_CLIENT_H_

#include <string>
#include <thread>
/*
class Message{
    char msgname[256];
    int msglen;
};
*/
using namespace std;

// The client class of rfss, each instance contains the information of the other
// end of the socked and the socked descriptor.
class Client {
  public:
    Client(int socketFD);
    ~Client();

    void Run();
    bool Send(char *data, size_t len);
    void Stop();

    bool SendFile(const string& filename_str);
    void PrintInfo();

    static int Connect(char* hostname, int portno);

    bool stop;
    bool dead;
    thread *client_thread;
    string host_name;
    string peer_name;
    string peer_ip;
//    struct hostent *peer_host;
//    struct sockaddr_in peer_add;
    int socketFD;
};

#endif  // _CS536_PA01_CLIENT_H_
