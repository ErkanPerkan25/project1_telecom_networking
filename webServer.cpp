#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// error codes that corresponds to socket problems
#define SUCCESS     0
#define USAGE_ERROR 1
#define SOCK_ERROR  2
#define BIND_ERROR  3
#define LISTEN_ERROR 4

#define MAX_WAITING 25

int doServer(int);
void doWork(int, struct sockaddr_in *);

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
    string request;

    char readBuffer[81];

    int charsRead = read(conn_sock, readBuffer, 80);

    readBuffer[charsRead] = '\0';

    if(readBuffer[charsRead-1] == '\n'){
        readBuffer[charsRead-2] = '\0';
        request = readBuffer;
    }

    /*
    vector<string> reqArr;
    string part;

    for(int i=0; i<request.length();i++){
        if (i == ' ') {
            part = request.substr(0,i);
            reqArr.push_back(part);

            request.replace(0,i,"");
        }
    }
    */

    cout << "Connection from " << inet_ntoa(client_addr->sin_addr) << endl
    << request << endl;

    close(conn_sock);
}



