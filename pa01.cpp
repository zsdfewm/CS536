#include<iostream>

using namespace std;

int main(int agrc, char * * argv)
{
    bool help = false;
    bool p_exit = false;
    bool myip = false;
    bool myport = false;
    bool connect = false;
    bool list = false;
    bool termination = false;
    bool fileGen = false;
    bool upload = false;
    bool creator = false;
    bool badSwitch = false;
    
    int port_num = atoi(argv[1]);
    
    while(!p_exit){
        if (help) {
            cout << "This application will take one command line parameter that correspnds to\n";
            cout << "the port on which your process will listen for incoming connections.\n\n";
            cout << "The following options are available: \n\n";
            cout << "help\n";
            cout << "   Display information about the available user interface options.\n";
            cout << "exit\n";
            cout << "   Close all connections and terminate this process.\n";
            cout << "myip\n";
            cout << "   Display the IP address of this machine.";
            cout << "myport\n";
            cout << "   Display the port on which this process is listening for incoming connections.";
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
        }
        if (p_exit){
            return EXIT_SUCCESS;
        }
        if (myip){
            return EXIT_SUCCESS;
        }
        if (myport){
            cout << "Port " << port_num << "is listening for all the connections\n";
        }
        if (connect) {
            return EXIT_SUCCESS;
        }
        if (list) {
            return EXIT_SUCCESS;
        }
        if (termination) {
            return EXIT_SUCCESS;
        }
        if (fileGen) {
            return EXIT_SUCCESS;
        }
        if (upload) {
            return EXIT_SUCCESS;
        }
        if (creator) {
            cout << "The creator of this application is Di Jin" <<endl;
            cout << "Email : jind@purdue.edu" <<endl;
        }
        if (badSwitch) {
            cout << "The option is not found... Aborting the program..." <<endl;
            return EXIT_FAILURE;
        }
    }
}
