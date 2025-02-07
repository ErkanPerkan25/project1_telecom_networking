#include <arpa/inet.h>
#include <cstdio>
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
    // Variables to store request information
    stringstream request;
    string method;
    string path;
    string version;

    char readBuffer[81];
    int charsRead; 
    int totalCharsRead;
    string finalBuffer;

    // reading in values
    while (true) {
        // read how many characters that have been read
        charsRead = read(conn_sock, readBuffer, 80);

        // if no characters read, stop
        if(charsRead <= 0)
            break;

        // read in chars, store them
        request << readBuffer;
        // stores how many characters that have been read total
        totalCharsRead += charsRead;

        // If there less than then the max that can be read, we are done
        if(charsRead < 80)
            break;
        
    }


    request.str()[totalCharsRead] = '\0';

    if(request.str()[totalCharsRead-1] == '\n'){
        request.str()[totalCharsRead-2] = '\0';
        //request << finalBuffer;
    }

    request >> method >> path >> version;

    // when method is GET, do the GET request
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

        // set path to the current directory
        path = "./" + path;
 
        // checks if the file exist by trying to open it
        fstream fs;
        fs.open(path);

        // if it is open, it exists, send the content
        if(fs.is_open()){
            // buffer to store content
            string buffer;
            
            // header information
            buffer += version + " 200 OK\n";
            buffer += "Content-Type: text/html; charset=utf-8\r\n\r\n";

            // as long as characters are read, add to buffer
            char ch;
            while(fs.get(ch)){
                buffer += ch;
            }

            // Close file when done
            fs.close();

            // the content in char pointer array form
            char *cbuff = (char *) buffer.c_str();

            // how much data is going to be sent to the client
            int needed = buffer.length();
            

            // send the content to the client
            while (needed) {
                int n = write(conn_sock, cbuff, needed);
                needed -= n;
                cbuff += n;
            }
            

        }
        // file does not exist, send error message
        else{
            // buffer for the content
            string buffer;
            // header information
            buffer = version + " 404 NOT FOUND\r\n\r\n";
            buffer += "<b>404 Error - resource not found on this server</b>";

            // converts string to char pointer array
            char *cbuff = (char *) buffer.c_str();

            // the total number of characters
            int needed = buffer.length();

            // send the content to the client
            while (needed) {
                int n = write(conn_sock, cbuff, needed);
                needed -= n;
                cbuff += n;
            }
        }

    }
    // does not now the method, return bad request
    else{
        cerr << "405 Method Not Allowed! Stopping the request!" << endl;
    }
    
    // Prints out the connection the server has accepted
    cout << "Connection from " << inet_ntoa(client_addr->sin_addr) << endl;

    // closes the socket
    close(conn_sock);
}



