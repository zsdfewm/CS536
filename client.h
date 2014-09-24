#ifndef _CS536_PA01_CLIENT_H_
#define _CS536_PA01_CLIENT_H_

#include <string>
#include <thread>

// The Client class of rfss. Whenever we get a socket created, we use the
// socket descriptor to create an instance of Client. Then starts a thread
// on non-blocking recv() until the socket is terminated or close by the peer. 
class Client {
  public:
    // Constructor with socket descriptor. Set peer_name and peer_ip.
    Client(int socketFD);

    // Loop over non-blocking recv(), until the socket is closed.
    void Run();

    // Send a data buffer to peer.
    bool Send(char *data, size_t len);

    // Close the socket, stop the recv().
    void Stop();

    // Send a file to peer.
    bool SendFile(const std::string& filename_str);

    // Print peer_name and peer_ip
    void PrintInfo();

    // Static method, called by rfss, return the sockedFD if succeed.
    static int Connect(char* hostname, int portno);

    bool stop;
    bool dead;
    std::thread *client_thread;
    std::string host_name;
    std::string peer_name;
    std::string peer_ip;
    int socketFD;
    static int PACKAGE_SIZE;
};

#endif  // _CS536_PA01_CLIENT_H_
