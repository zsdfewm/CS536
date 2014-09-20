#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include <strings.h>
#include <iostream>
#include <thread>
//#include <netdb.h>
//#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <fcntl.h>


#include "server.h"
#include "socket_pool.h"

using namespace std;

Server::Server(SocketPool *socket_pool_p, int portno) {;
    socket_pool = socket_pool_p;
    port = portno;
    stop=false;
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
    cout << "Server is running on: " << port <<endl;
    clilen = sizeof(cli_addr);
    //int counter=0;
    while(stop == false) {
	// try to accept a client;
        //counter++;
	clisocketFD = accept(s_socketFD, (struct sockaddr *)&cli_addr, &clilen);

	if (clisocketFD < 0) {
            if (errno == EWOULDBLOCK) {
            } else {
//sleep(1);
//  printf("%x, stop = %d\n", this, stop);
	        cout << "ERROR on accept" <<endl;
            }
	} else {
           cout << "haa, got one!" << endl;
           if (!socket_pool->AddClient(clisocketFD)) {
             cout << "Server socket pool full, closing the socket." << endl;
             close(clisocketFD);
           }
        }
    }
//    exit(-1);
//    printf("Stops!!!!!\n");
//    fflush(stdout);
    //when terminate is called
    cout<<"Server stops"<<endl;

    // release server sockets;
    close(s_socketFD);
}
void Server::Stop() {
  this->stop = true;
//  printf("%x, stop = %d\n", this, stop);
}


/*******/
