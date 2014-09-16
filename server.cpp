#include "server.h"

#include <fcntl.h>
using namespace std;
//ServerWorker;

ServerWorker::ServerWorker(int csFD) {
    client_socketFD=csFD;
}
void ServerWorker::stop() {
    ::close(client_socketFD);
}
void ServerWorker::swread() {
    int len;
    //int n;
    char buff[512];
      
    bzero(buff,512);
    
    len = read(client_socketFD,buff,512);
    cout<<"Testing"<<endl;
    for(int i=0; i<len; i++)
    {
	cout<<buff[i];
	cout.flush();
    }
    cout<<endl;

    string buffer(buff);
    if (len < 0) 
	cout << "ERROR reading from socket" <<endl;
    cout << "len = " <<len << endl;
    printf("msg=%s\n",buff);
    cout.flush();
    len = write(client_socketFD,"I got your message",18);
    if (len < 0)
	cout << "ERROR writing to socket" <<endl;
}

Server::Server() {
    stop=false;
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
    fcntl(s_socketFD, F_SETFL, O_NONBLOCK);
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

    listen(s_socketFD, 5);
    cout << "Server is running" <<endl;
    clilen = sizeof(cli_addr);
    //int counter=0;
    while(stop == false) {
	// try to accept a client;
        //counter++;
	clisocketFD = accept(s_socketFD, (struct sockaddr *) & cli_addr, &clilen);

	if (clisocketFD < 0) {
            if (errno == EWOULDBLOCK) {
            } else {
	        cout << "ERROR on accept" <<endl;
            }
	} else {

	    //cout << "Testing..." <<

	    struct sockaddr *c_addr = new(struct sockaddr);
	    bcopy((struct sockaddr *) & cli_addr, (struct sockaddr *) c_addr, clilen);
	    cli_addr_vec.push_back(c_addr);

	    //tmpAddrPtr=&((struct sockaddr_in *)caddr)->sin_addr.s_addr;
	    cout << "!!!0Test ..." << cli_addr.sin_addr.s_addr <<endl;
	    cout << "!!!1Test ..." << &((struct sockaddr_in *)c_addr)->sin_addr.s_addr <<endl;

	    ServerWorker *server_worker = new ServerWorker(clisocketFD);
	    std::thread *server_worker_thread = new thread(&ServerWorker::swread, server_worker);
	    server_worker_vec.push_back(server_worker);
	    server_worker_thread_vec.push_back(server_worker_thread);
        }
    }
    //when terminate is called
    cout<<"Server stops"<<endl;

    for(ServerWorker *server_worker : server_worker_vec) {
      server_worker->stop();
    }
    // release all sockets;
    close(s_socketFD);
}
void Server::exit() {
  this->stop = true;
}


/*******/
