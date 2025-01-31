#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// error codes that corresponds to socket problems
#define SUCCESS     0
#define USAGE_ERROR 1
#define SOCK_ERROR  2
#define BIND_ERROR  3
#define LISTEN_ERROR 4

#define HTTP_METHOD_ERROR 5

#define MAX_WAITING 25

int doServer(int);
void doWork(int, struct sockaddr_in *);
// read function?
// write function?

int main(int argc, char *argv[]){
    
    if(argc!=2){
        cerr << "Usage: " << argv[0] << " <port number>" << endl;
        exit (USAGE_ERROR);
    }

    return doServer(stoi(argv[1]));
}

int doServer(int portNum){
    // the listenting socket
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);

    // If socket() fails, we exit
    if(listen_sock<0){
        cerr << "Could not create listenting socket!" << endl;
        return SOCK_ERROR;
    }

    //
    struct sockaddr_in local_addr;

    // Fill in local (server) half of socket info
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(portNum);

    // bind (i.e. "fill in") our socket info to server socket
    if(bind(listen_sock,(sockaddr*)&local_addr, sizeof(local_addr)) != 0){
        cout << "Binding failed - this could be caused by:" << endl
        << "    * an invalid port (no access, or already in use?)" << endl
        << "    * an invalid local address (did you use the wildcard?)"
        << endl;
        
        return BIND_ERROR;
    }

    // listen for a client connection on
    if(listen(listen_sock, MAX_WAITING) != 0){
        cout << "Listen error" << endl;
        return LISTEN_ERROR;
    }

    // keep handling connections forever
    while (true) {
        int connected_sock; // socket for the actual connected client
        struct sockaddr_in from; // holds client address data
        unsigned int from_len; // holds the size of client address data
    
        from_len = sizeof(from);

        // wait for the listening socket to get an attempetd
        // client connection
        connected_sock = accept(listen_sock,
                                (struct sockaddr*)&from,
                                &from_len);

        // get and process attempted client connection
        doWork(connected_sock, &from);
    }

    return SUCCESS;
}

void doWork(int conn_sock, struct sockaddr_in *client_addr){
    stringstream request;
    string method;
    string path;
    string version;


    char readBuffer[81];

    int charsRead = read(conn_sock, readBuffer, 80);

    readBuffer[charsRead] = '\0';

    if(readBuffer[charsRead-1] == '\n'){
        readBuffer[charsRead-2] = '\0';
        request << readBuffer;
    }

    request >> method >> path >> version;

    cout << method << endl;

    if (method == "GET") {
        // remove leadings .'s and /'s from the request
        for(int i=0; i < path.length(); i++){
            if(path[i] == '.' || path[i] == '/'){
                path.erase(path.begin()+i);
                i--;
            }
            else{
                break;
            }
        }

        path = "./" + path;
 
        fstream fs;
        fs.open(path);
        if(fs.is_open()){
            string buffer;

            buffer = version + " 200 OK\n";
            buffer += "Content-type:";
            buffer += "\r\n\r\n";

            while(!fs.eof()){
                buffer += fs.get();
            }

            char *cbuff = (char *) buffer.c_str();

            int needed = buffer.length();

            while (needed) {
                int n = write(conn_sock, cbuff, needed);
                needed -= n;
                cbuff += n;
            }

        }
        else{
            string buffer;
            buffer = version + " 404 NOT FOUND";
            buffer += "\r\n\r\n";
            buffer += "<b>404 Error - resource not found on this server</b>";

            char *cbuff = (char *) buffer.c_str();

            int needed = buffer.length();

            while (needed) {
                int n = write(conn_sock, cbuff, needed);
                needed -= n;
                cbuff += n;
            }
        }

    }
    else{
        cerr << "405 Method Not Allowed! Stopping the request!" << endl;
    }
    
    cout << "Connection from " << inet_ntoa(client_addr->sin_addr) << endl;

    close(conn_sock);
}



