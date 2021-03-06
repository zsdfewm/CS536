#include <netinet/in.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <iostream>
#include <errno.h>
#include <stdint.h>
#include <limits>
#include <fcntl.h>
#include <sys/time.h>

#include "client.h"

#define STATUS_IDLE 0
#define STATUS_BUSY 1

//#define PACKAGE_SIZE 512

using namespace std;

char kMagicNumber[] = "31415926";
size_t MAGIC_SIZE = 8;


int Client::PACKAGE_SIZE = 512;

Client::Client(int socketFD_p) {
    socketFD = socketFD_p;
    stop = false;
    dead = false;
    struct sockaddr addr;
    socklen_t addrlen = sizeof(addr);
    getpeername(socketFD, &addr, &addrlen);

    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    gethostname(host, NI_MAXHOST);
    host_name = host;
    getnameinfo(&addr, addrlen, host, sizeof(host), serv, sizeof(serv), 0);
//  printf("host = %s, serv = %s\n", host, serv);
    fcntl(socketFD, F_SETFL, O_NONBLOCK);
    peer_name = host;
    getnameinfo(&addr, addrlen, host, sizeof(host), serv, sizeof(serv), NI_NUMERICHOST);
    peer_ip = host;

}


void Client::Stop() {
    stop = true;
}


//enable client's ability to accept file
void Client::Run(){
    char buff[10000];
    int len;
    FILE *fp;
    int status = STATUS_IDLE;
    int filesize;
    int written_size;
    char filename[1000];
    struct timeval tv_start, tv_now;
    double time_elapsed;
cout << "client running" << endl; 
    while(stop == false){
        len = recv(socketFD, buff, PACKAGE_SIZE, 0);
        if (len == -1){
//          printf("errno = %x\n", errno);
            if (errno != EWOULDBLOCK){
                cout << "ERROR on read, stop socket" << endl;
                break;
            }
        } 
        else{
            if (len != 0) {
//          printf("message len = %d\n", len);
		//the status is idle means client can handle the file rev right now
		//otherwise, may in the process of copying other file
                if (status == STATUS_IDLE) {
                    buff[len]=0;
//                  printf("Got message: %s\n", buff);
                    bool check = true;
                    if (len < ((int) (MAGIC_SIZE+ sizeof(int)+1))) {continue;}
                    for(size_t i = 0; i < MAGIC_SIZE; ++i) {
			//check if this package contains magic #
                        if (buff[i] != kMagicNumber[i]) {
                            check = false;
                        }
                    }
		    //if this is not the waited package, just ignore this package
                    if (!check) {continue;}
		    //get the filesize via an int pointer
                    filesize =* (int*)(&buff[8]);
		    //get the file name
	            size_t l = MAGIC_SIZE+sizeof(int);
	            size_t i = 0;
	            for(i = 0; ((int) l) < len; ++i, ++l) {
	                filename[i]=buff[l];
	            }
	            filename[i] = 0;

		    //set the status to busy to indicate the client is copying file
	            status = STATUS_BUSY;
	            printf("Rx: receving file %s, size=%d\n", filename, filesize);
	            //int result = mkdir("./upload", 0777);
		    mkdir("./Upload", 0777);
	            fp = fopen(filename, "w");
	            if (fp == NULL) {
	                printf("Rx: Can not open local file: %s\n", filename);
	                continue;
	            }
	            written_size = 0;
	            gettimeofday(&tv_start, NULL);
	        } else if (status == STATUS_BUSY) {
		    //in the process of receiving a file
                    if (written_size + len < filesize) {
	                if (len != ((int) fwrite(buff, sizeof(char), len, fp))) {
	                    printf("Rx: Error write local file: %s\n", filename);
	                } 
			else {
	                    written_size += len;
	                    gettimeofday(&tv_now, NULL);
	                    time_elapsed = (tv_now.tv_sec - tv_start.tv_sec)*1000000 +
	                    (double)(tv_now.tv_usec - tv_start.tv_usec);
			    //printf("\rGetting %s: %d Bytes in %.5f Sec\n", filename, written_size, time_elapsed);
	                }
	            } else {
			//receive the last package of the file data
		        if (filesize - written_size != ((int) fwrite(buff, sizeof(char), filesize - written_size, fp))) {
		            printf("Local file written error");
		        } else {
	                    written_size = filesize;
	                    time_elapsed = (tv_now.tv_sec - tv_start.tv_sec)*1000000 +
	                    (double)(tv_now.tv_usec - tv_start.tv_usec);
			    //printf("\rGetting %s: %d Bytes in %.5f Sec\n", filename, written_size, time_elapsed);
	                    cout << "Rx(" << host_name << "):";
	                    cout << peer_name << "->" << host_name << endl;
	                    cout << "  File size: " << written_size << "Bytes" << endl;
	                    cout << "  Time taken: " << time_elapsed << "useconds, " << endl;
	                    cout << "  Rx rate: "<< 8*1000000*(written_size/time_elapsed) << "bits/second" << endl;
	                    fclose(fp);
	                    status = STATUS_IDLE;
	                }
		    }
		}
	    } else {
		//cout << "Length 0 message (closed channel?)" << endl;
		break;
	    }
	}
    }
	    //printf("socket %x closed\n", socketFD);
    close(socketFD);
    dead = true;
}

bool Client::Send(char *data, size_t len) {
    int sent_len = 0;
    int tmp_len = 0;  // send(socketFD, data, len, 0);
    while(sent_len < ((int)len)) {
        tmp_len = send(socketFD, &data[sent_len], len-sent_len, 0);
        if (tmp_len < 0) {
        //do nothing
        } else {
            sent_len += tmp_len;
            if (tmp_len != ((int)len)) {
                printf("Sent %d byte, total %d/%d\n", tmp_len, sent_len, len);
            }
        }
    }
    return true;
}

void padding(char* buff, int used_len) {
    for(int l = used_len; l < Client::PACKAGE_SIZE; ++l) {
        buff[l]=0;
    }
}

bool Client::SendFile(const string& file_name) {
//    int result = mkdir("./Upload", 0777);
    mkdir("./Upload", 0777);
    string filename = "./Upload/" + file_name;
    char* c_filename = new char[filename.size()+1];
    memcpy(c_filename, filename.c_str(), filename.size());
    c_filename[filename.size()] = 0;
    int filesize;
    FILE *fp = fopen(c_filename, "r");
    if (fp == NULL) {
        printf("Can not open file: %s.\n", c_filename);
        return false;
    }
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    printf("reading file %s, size = %d\n", c_filename, filesize);
    char buff[PACKAGE_SIZE];
    struct timeval tv_start, tv_now;
    double time_elapsed;

    gettimeofday(&tv_start, NULL);
// 1: send magic number#1+filesize+filename
    size_t l=0;
    size_t i=0;
    while(i<MAGIC_SIZE) {
        buff[l++] = kMagicNumber[i++];
    }
    char *tmp=(char*) (&filesize);
    i=0;
    while(i<sizeof(int)) {
        buff[l++]=tmp[i++];
    }
    i=0;
    while(i<filename.size()) {
        buff[l++]=filename.c_str()[i++];
    }
    buff[l]=0;
//  for(i=0;i<l;i++){
//   printf("%c", buff[i]);
//  }
//  printf("\n");
    padding(buff, l);
    if (!Send(buff, PACKAGE_SIZE)) {
        cout << "Initializing file transmittion error." <<endl;
        fclose(fp);
        return false;
    }
// 2: send file contents
    int written_size = 0;
    int read_len;
    while ((read_len = fread(buff, sizeof(char), PACKAGE_SIZE, fp)) > 0) {
        padding(buff, read_len);
        while (!Send(buff, PACKAGE_SIZE)) {
        }
        gettimeofday(&tv_now, NULL);
        written_size += read_len;
        time_elapsed = (tv_now.tv_sec - tv_start.tv_sec)*1000000 +
	                    (double)(tv_now.tv_usec - tv_start.tv_usec);
//    printf("\rSend %d Bytes in %.5f Sec", written_size, time_elapsed);
    }
    cout << "Tx(" << host_name << "): ";
    cout << host_name << "->" << peer_name << endl;
    cout << "  File size: " << written_size << "Bytes" << endl;
    cout << "  Time taken: " << time_elapsed << "useconds" << endl;
    cout << "  Tx rate: " << 8*1000000*(written_size/time_elapsed) << "bits/second" << endl;
    fclose(fp);
    return true;
}

void Client::PrintInfo() {
//  cout << "dummy info" << endl;
    cout << peer_name << " : " << peer_ip << endl; 
}


int Client::Connect(char* hostname, int portno) {
    struct hostent* server = gethostbyname(hostname);
    if (server == NULL) {
        printf("ERROR, no such host: %s \n", hostname);
    }
    int port = portno;
    struct sockaddr_in serv_addr;

    int c_socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (c_socketFD < 0) 
        cout << "ERROR opening socket" <<endl;

    bzero((char *) &serv_addr, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
       (char *)&serv_addr.sin_addr.s_addr,
       server->h_length);
    serv_addr.sin_port = htons(port);
//  printf("trying to connect %s @ %d\n", hostname, port);
    if (connect(c_socketFD,
              (struct sockaddr *) &serv_addr,
              sizeof(struct sockaddr_in)) < 0) {
        cout << "ERROR connecting" << endl << strerror(errno) <<endl;
    }
//    send(c_socketFD, "hello channel", 100, 0);
    return c_socketFD;
}


