/*
    Name: HUSSAIN SADIQ ABUWALA
    STUDENT-ID: 27502333
    HIGH-LEVEL-DESCRIPTION - client program that sends messages to server.

*/


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCKETNAME "/tmp/a2-27502333.socket"

int main(int argc, char *argv[]){

    char buffer[1024];
    int n, sock, len;
    struct sockaddr_un name;
    

    //create a socket
    
    if((sock = socket(AF_UNIX, SOCK_STREAM,0)) < 0){
        perror("client: socket");
        exit(1);
    }

    //Create the address of the server

    memset(&name, 0, sizeof(struct sockaddr_un));
    name.sun_family = AF_UNIX;
    strcpy(name.sun_path, SOCKETNAME);
    len = sizeof(name.sun_family) + strlen(name.sun_path);

    
    //connect to server
    if(connect(sock, (struct sockaddr *) &name, SUN_LEN(&name)) < 0){
        perror("client: connect");
        exit(1);
    }
    
    strcpy(buffer,argv[0]);
    
    //send client header message to server.
    send(sock,buffer,strlen(buffer) + 1,0);

    //keep reading data while there is input at stdin
    while((n = read(0,buffer,sizeof(buffer))) > 0){
        
        send(sock,buffer,n,0);
        
    }    

    close(sock);
    exit(0);
}
