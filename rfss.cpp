#include "rfss.h"

using namespace std;

/*
Rfss::Rfss(char * hostname, int port) {
    //Client *cli = new Client();
    //cli->init(hostname, port);
    client.init(hostname, port);
    server.init(port);	//serverpart

    cout << "test... " << hostname <<endl;
    cout << "test... " << port <<endl;
}
*/
/**/
void Rfss::init(char * hostname, int port) {
    //client.init(hostname, port);
    server.init(port);	//serverpart
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
    cout<<"Connect to " << hostname << " on Port "<<port <<endl;
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

/* terminate all connections and exit the process */
void Rfss::exit(){
    cout<<"sleep for 3 sec"<<endl;
    sleep(3);
    server.exit();
    for(Client *client : client_vec) {
      client->stop();
    }
    s_thread->join();
}

/* display the ip of local machine */
void Rfss::GetMyIp() {
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    //cout<<"Testing: AF_INET=" << AF_INET <<endl;

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
	//cout << "name:" << ifa->ifa_name<< "\tsa_family: " << ifa->ifa_addr->sa_family <<endl;

        if (!(ifa->ifa_addr)) {
            continue;
        }

        else if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address

	    //cout << ifa->ifa_name << " equal to AF_INET" << endl;

            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
	    cout << "Test ..." << tmpAddrPtr <<endl;

            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("IPv4 %s IP Address %s\n", ifa->ifa_name, addressBuffer);
	    //freeifaddrs(ifAddrStruct);
	    //return true;
        } 
    }
    if (ifAddrStruct!=NULL) 
    {
	freeifaddrs(ifAddrStruct);
	ifAddrStruct = NULL;
    }
    //return false;
}


void Rfss::ShowList() {
    //unsigned int i=0;
    void * tmpAddrPtr=NULL;
    //Client * c = client_vec.front();
    int j=1;
/*
    for(vector<Client *>::iterator i = client_vec.begin(); 
	    i!=client_vec.end(); i++)
    {
	//cout << i << " : " << ;
	tmpAddrPtr=&(c->client_add.sin_addr);

	cout << "Test..." << tmpAddrPtr <<endl;

        char addressBuffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
        //printf("IPv4 %s IP Address %s\n", ifa->ifa_name, addressBuffer);
	//cout << i << " : " << addressBuffer <<endl;
	printf("%d : %s\n", j, addressBuffer);
	j++;
    }
*/

    cout<<"size ="<<client_vec.size()<<endl;
    for(Client *c : client_vec) {
	cout<<&(c->client_add).sin_addr;
    }
}

void Rfss::GenerateFile(char* file_name, int file_size) {
}
