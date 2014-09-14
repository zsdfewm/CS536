#include "rfss.h"

using namespace std;

/**/
Rfss::Rfss(char * hostname, int port) {
    //Client *cli = new Client();
    //cli->init(hostname, port);
    client.init(hostname, port);
    //server.init(port);	//serverpart

    cout << "test... " << hostname <<endl;
    cout << "test... " << port <<endl;
}
/**/

void Rfss::init() {
    // Let the server begin to listen
    //thread server_thread(&Server::svlisten, server);	//serverpart
    // Take control of terminal

    //cout << "test... before join..." <<endl;
    //server_thread.join();	//serverpart

    
  
}

// Connected to a host;
void Rfss::connect(char * host_name, int port) {
    Client *cli = new Client();
    cli->init(hostname, port);
    //client.init(hostname, port);
    if (cli->connect()) {
	client_vec.append(cli);
	std::cout << "Connect success";
    } else {
	std::cout << "Connect failed";
    }
}
/*
void Rfss::terminate(int connect_id) {
    Client *client = client_vec.erase(connect_id-1);
    client->stop();
    delete client;
}

*/

int main(int argc, char * * argv) {
    //int port = 12345;
    char * hostname = argv[1];
    int port = atoi(argv[2]);
    Rfss rfss(hostname, port);
    rfss.init(); 
    rfss.connect(hostname, port);
    //rfss.terminate();
}
