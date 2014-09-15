#include "rfss.h"

using namespace std;

/**/
Rfss::Rfss(char * hostname, int port) {
    //Client *cli = new Client();
    //cli->init(hostname, port);
    client.init(hostname, port);
    server.init(port);	//serverpart

    cout << "test... " << hostname <<endl;
    cout << "test... " << port <<endl;
}

/**/
void Rfss::init() {
    // Let the server begin to listen
    s_thread = new thread (&Server::svlisten, &server);	//serverpart
    // Give the server 3 second to prepare
    sleep(3);

    // Take control of terminal

    //cout << "test... before join..." <<endl;
    //server_thread.join();	//serverpart

    
}

// Connected to a host;
void Rfss::connect(char * hostname, int port) {
    Client *cli = new Client();
    cli->init(hostname, port);
    cout<<"Connect to " << hostname << "on Port "<<port <<endl;
    //client.init(hostname, port);
    //if (cli->connect()) {
    client_vec.push_back(cli);
    cli->upload();
    //cli->stop();
}

/* terminate one connection with host */
void Rfss::terminate(int connect_id) {
    Client *cli = client_vec[connect_id-1];
    cli->stop();
    delete cli;
    client_vec.erase(client_vec.begin()+connect_id-1);
}

void Rfss::exit(){
    cout<<"sleep for 3 sec"<<endl;
    sleep(3);
    server.exit();
    for(Client *client : client_vec) {
      client->stop();
    }
    s_thread->join();
}

int main(int argc, char * * argv) {
    //int port = 12345;
    char * hostname = argv[1];
    int port = atoi(argv[2]);
    int port2 = atoi(argv[3]);
    Rfss rfss(hostname, port);
    Rfss rfss2(hostname, port2);
    rfss.init(); 
    rfss2.init();
    rfss.connect(hostname, port2);
    rfss2.connect(hostname, port);
    rfss.exit();
}
