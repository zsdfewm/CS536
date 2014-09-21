#include<iostream>
#include<string>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "rfss.h"

using namespace std;


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

    string hostname;
    int port;
    string filename;
    int filesize;

    int con_id;

    //int i=0;

    Rfss rfss;

    if(argc<2){
	cout << "Missing element from the command!" <<endl;
	return EXIT_FAILURE;
    }
    char c_hostname[1024];
    c_hostname[1023] = 0;
    gethostname(c_hostname, 1023);
    int port_num = atoi(argv[1]);
    printf("localhost name: %s\n", c_hostname);
    rfss.Init(c_hostname, port_num); 
    cout << "Please input your option... Type help for assistance..." <<endl;

    string option;
    cin >> option;
    char *opt = new char[option.size()+1];
    memcpy(opt, option.c_str(), option.size());
    opt[option.size()] = '\0';
    while(true){

	//for(i=2; i<argc; i++)
	//{
	//Check which option is input by user
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
	    rfss.Stop();
            return EXIT_SUCCESS;
        }
        if (showMyip){
	    showMyip = false;
	    //IPv4 *myip = new(IPv4);
	    rfss.GetMyIp();
            //return EXIT_SUCCESS;
        }
        if (myport){
	    myport = false;
            cout << "Port " << port_num << " is listening for all the connections\n";
        }
        if (portConnect) {
	    portConnect = false;
	    cin >> hostname;
	    cin >> port;
	    rfss.Connect(hostname, port);
            //return EXIT_SUCCESS;
        }
        if (showList) {
	    showList = false;
	    rfss.ShowList();
            //return EXIT_SUCCESS;
        }
        if (conTerminate) {
            cin >> con_id;
            rfss.Terminate(con_id);
	    conTerminate = false;
            //return EXIT_SUCCESS;
        }
        if (fileGen) {
            cin >> filename;
            cin >> filesize;
	    fileGen = false;
            rfss.GenerateFile(filename, filesize);
            //return EXIT_SUCCESS;
        }
        if (conUpload) {
            cin >> con_id;
            cin >> filename;
            rfss.Upload(con_id, filename);
	    conUpload = false;
            //return EXIT_SUCCESS;
        }
        if (creator) {
	    creator = false;
            cout << "The creator of this application is Di Jin" <<endl;
            cout << "Email : jind@purdue.edu" <<endl;
        }
        if (badSwitch) {
	    badSwitch = false;
            cout << "The option is not found... Please check..." <<endl;
//          return EXIT_FAILURE;
        }

	cout << endl<<"Please input your option..." <<endl;
	cin >> option;
	//char *opt = new char[option.size()+1];
	memcpy(opt, option.c_str(), option.size());
	opt[option.size()] = '\0';
    }
}
