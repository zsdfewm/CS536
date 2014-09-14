#include "server.h"

using namespace std;
//ServerWorker;

ServerWorker::ServerWorker(int csFD) {
    client_socketFD=csFD;
}

void ServerWorker::swread() {
    int len;
    //int n;
    char buffer[512];
      
    bzero(buffer,512);
    
    len = read(client_socketFD,buffer,512);
    if (len < 0) 
	cout << "ERROR reading from socket" <<endl;
    cout <<"Here is the message: " << buffer <<endl;
    len = write(client_socketFD,"I got your message",18);
    if (len < 0)
	cout << "ERROR writing to socket" <<endl;
}
void Server::init(int portno) {
    port = portno;
}

/**************/
void Server::svlisten() {
    //init server socket
    //listen to the port
    int clisocketFD;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    //cout << "Testing... Coming to svlisten()" << endl;

    s_socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(s_socketFD < 0){
	cout << "ERROR Opening socket"<<endl;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    //bind the server socket to the listen port
    int bd = ::bind(s_socketFD, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr));
    if ( bd < 0)
	cout << "ERROR on binding"<<endl;

    listen(s_socketFD, 12345);
    clilen = sizeof(cli_addr);
    while(stop == false) {
	// try to accept a client;
	clisocketFD = accept(s_socketFD, (struct sockaddr *) & cli_addr, &clilen);
	if (clisocketFD < 0) {
	    cout << "ERROR on accept" <<endl;
	}
	ServerWorker *server_worker = new ServerWorker(clisocketFD);
	std::thread *server_worker_thread = new thread(&ServerWorker::swread, server_worker);
	server_worker_vec.push_back(server_worker);
	server_worker_thread_vec.push_back(server_worker_thread);
        server_worker->swread();
    }

    //when terminate is called
    exit(0);  
}

/*******/
