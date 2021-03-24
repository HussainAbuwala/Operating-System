/*
    Name: HUSSAIN SADIQ ABUWALA
    STUDENT-ID: 27502333
    HIGH-LEVEL-DESCRIPTION - The client can send messages to the server and the messages will be displayed in the server side inside the client box.

*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCKETNAME "/tmp/a2-27502333.socket"            //name of the socket

int main(int argc, char *argv[]){

    printf("<program.push                           \n"         //adding background color to the iol terminal
            "       background-color = x00FFFF      \n"
            ">                                      \n"
          );

    char buffer[1024];                          
    int n, sock, nsock, len;
    struct sockaddr_un name;

    //create a socket
                                                                                    
    if((sock = socket(AF_UNIX, SOCK_STREAM,0)) < 0){            //create a socket
        perror("server: socket");
        exit(1);
    }

    //Create the address of the server

    memset(&name, 0, sizeof(struct sockaddr_un));
    name.sun_family = AF_UNIX;
    strcpy(name.sun_path, SOCKETNAME);
    len = sizeof(name.sun_family) + strlen(name.sun_path);


    //remove any previous socket.

    unlink(name.sun_path);

    //bind the socket to the address

    if(bind(sock, (struct sockaddr*) &name, SUN_LEN(&name)) < 0){
        perror("server: bind");
        exit(1);
    }

    //listen for connections

    if(listen(sock, 5) < 0){
        perror("server: listen");
        exit(1);
    }

    
    //Accept a connection

    if((nsock = accept(sock, (struct sockaddr *) &name, &len)) < 0){
        perror("server: accept");
        exit(1);
    }
    
   // creating a client box to the iol terminal
   printf(
            "<client:box                    \n"
            "    background-color = xFFFFFF \n"
            "    width = 100,{%}            \n"
            "    visible = true             \n"
            ">                              \n"
    );
    
    //receive client message
    recv(nsock,buffer,sizeof(buffer),0);

    //add client message to client box
    printf(
            "<client.push <box              \n"
            "    color = xFFFFFF            \n"
            "    background-color = {blue}  \n"
            "    {%s}                       \n"
            "    width = 100,{%}            \n"
            "    visible = true             \n"
            ">>                             \n"
    ,buffer);
    
    //clear the buffer
    memset(&buffer, 0, sizeof(buffer));
    
    //receive from client until the client has nothing to send and quits 
    while((n=recv(nsock,buffer,sizeof(buffer),0)) > 0){
        //buffer[n] = "\0";
        strcat(buffer,"\0");
        //add client message to client box
        printf(
            "<client.push                   \n"
            "    color = {red}              \n"
            "    {%s</n>}                   \n"
            ">                              \n"
        ,buffer);

        //clear the buffer
        memset(&buffer, 0, sizeof(buffer));
        
    }        
    
    //close the socket
    close(nsock);
    close(sock);
    exit(0);
}
