#include"client.h"

using namespace std;

//std::numeric_limits<int32_t>::max();

// Initialize the client to the server;
void Client::init(char * hostname, int portno)
{
    
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    server_port = portno;
}

// Upload the file to the server;
void Client::upload()
{
    struct sockaddr_in serv_addr;
    //char buffer[512];
    string buffer;
    int len;

    c_socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (c_socketFD < 0) 
        cout << "ERROR opening socket" <<endl;
    //server = 
    
    bzero((char *) &serv_addr, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(server_port);
    if (connect(c_socketFD,(struct sockaddr *) &serv_addr,sizeof(struct sockaddr_in)) < 0) 
        cout << "ERROR connecting" << endl << strerror(errno) <<endl;
    cout << "Please enter the message: " <<endl;  

    cin >> buffer;
    cout << "Testing... " << buffer <<endl;
    char *buff = new char[buffer.size()+1];
    buff[buffer.size()]=0;
    memcpy(buff, buffer.c_str(), buffer.size());


    len = write(c_socketFD,buff,buffer.size()+1);
    if (len < 0) 
         cout << "ERROR writing to socket" <<endl;
    bzero(buff,256);
    len = read(c_socketFD,buff,255);
    if (len < 0) 
         cout << "ERROR reading from socket" <<endl;
    cout << "The message is: " << buff <<endl;
}


// Stop the connection to the server;
void Client::stop()
{
    close(c_socketFD);
}
