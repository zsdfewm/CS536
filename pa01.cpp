#include<iostream>
#include<string>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/types.h>
#include<ifaddrs.h>
#include<arpa/inet.h>
using namespace std;

bool GetMyIp() {
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("IPv4 %s IP Address %s\n", ifa->ifa_name, addressBuffer);
	    freeifaddrs(ifAddrStruct);
	    return true;
        } 
	else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("IPv6 %s IP Address %s\n", ifa->ifa_name, addressBuffer); 
        } 
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    return false;
}

int main(int argc, char * * argv)
{
    bool help = false;
    bool p_exit = false;
    bool showMyip = false;
    bool myport = false;
    bool portConnect = false;
    bool showList = false;
    bool conTerminate = false;
    bool fileGen = false;
    bool conUpload = false;
    bool creator = false;
    bool badSwitch = false;
    
    if(argc<2){
	cout << "Missing element from the command!" <<endl;
	return EXIT_FAILURE;
    }
    int port_num = atoi(argv[1]);
    
    cout << "Please input your option... Type help for assistance..." <<endl;

    string option;
    cin >> option;
    char *opt = new char[option.size()+1];
    memcpy(opt, option.c_str(), option.size());
    opt[option.size()] = '\0';
    while(true){

	//for(i=2; i<argc; i++)
	//{
	if(strcmp(opt, "help") == 0)
	    help = true;
	else if(strcmp(opt, "exit") == 0)
	    p_exit = true;
	else if(strcmp(opt, "myip") == 0)
	    showMyip = true;
	else if(strcmp(opt, "myport") == 0)
	    myport = true;
	else if(strcmp(opt, "connect") == 0)
	    portConnect = true;
	else if(strcmp(opt, "list") == 0)
	    showList = true;
	else if(strcmp(opt, "terminate") == 0)
	    conTerminate = true;
	else if(strcmp(opt, "generate") == 0)
	    fileGen = true;
	else if(strcmp(opt, "upload") == 0)
	    conUpload = true;
	else if(strcmp(opt, "creator") == 0)
	    creator = true;
	else
	{
	    badSwitch = true;
	    cout << stderr << "Unknown Switch: '" << option <<"'\n";
	}
	//}

        if (help) {
            cout << "This application will take one command line parameter that correspnds to\n";
            cout << "the port on which your process will listen for incoming connections.\n\n";
            cout << "The following options are available: \n\n";
            cout << "help\n";
            cout << "   Display information about the available user interface options.\n";
            cout << "exit\n";
            cout << "   Close all connections and terminate this process.\n";
            cout << "myip\n";
            cout << "   Display the IP address of this machine.\n";
            cout << "myport\n";
            cout << "   Display the port on which this process is listening for incoming connections.\n";
            cout << "connect [host_name] [port]\n";
            cout << "   Establish a new connection to the machine with the specified name that is listening\n";
            cout << "   on the specified port (3456)\n";
            cout << "list\n";
            cout << "   Display a numbered list of all the connections that this process is part of.\n";
            cout << "terminate [connection_ID]\n";
            cout << "   This will terminate the connection listed under number 2 when LIST is used to display\n";
            cout << "   all connections.\n";
            cout << "generate [file_name] [file_size]\n";
            cout << "   This will generate a file which is located under the local path Upload/ with the \n";
            cout << "   specified name and size (contents do not matter).\n";
            cout << "upload [connection_ID] [upload_file_name]\n";
            cout << "   This will upload the file named upload file name which is located under the local\n";
            cout << "   path Upload/ to the host on the connection that is designated by the number 4 when\n";
            cout << "   LIST is used.\n";
            cout << "creator\n";
            cout << "   Display your (the student’s) full name and Purdue email address.\n\n";
	    help = false;
        }
        if (p_exit){
	    p_exit = false;
            return EXIT_SUCCESS;
        }
        if (showMyip){
	    showMyip = false;
	    //IPv4 *myip = new(IPv4);
	    if(!GetMyIp())
            	return EXIT_FAILURE;
        }
        if (myport){
	    myport = false;
            cout << "Port " << port_num << " is listening for all the connections\n";
        }
        if (portConnect) {
	    portConnect = false;
            return EXIT_SUCCESS;
        }
        if (showList) {
	    showList = false;
            return EXIT_SUCCESS;
        }
        if (conTerminate) {
	    conTerminate = false;
            return EXIT_SUCCESS;
        }
        if (fileGen) {
	    fileGen = false;
            return EXIT_SUCCESS;
        }
        if (conUpload) {
	    conUpload = false;
            return EXIT_SUCCESS;
        }
        if (creator) {
	    creator = false;
            cout << "The creator of this application is Di Jin" <<endl;
            cout << "Email : jind@purdue.edu" <<endl;
        }
        if (badSwitch) {
	    badSwitch = false;
            cout << "The option is not found... Aborting the program..." <<endl;
            return EXIT_FAILURE;
        }

	cout << endl<<"Please input your option..." <<endl;
	cin >> option;
	//char *opt = new char[option.size()+1];
	memcpy(opt, option.c_str(), option.size());
	opt[option.size()] = '\0';
    }
}
