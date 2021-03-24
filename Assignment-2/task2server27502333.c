/*
    Name: HUSSAIN SADIQ ABUWALA
    STUDENT-ID: 27502333
    HIGH-LEVEL-DESCRIPTION - -- The client can send messages to the server and the messages will be displayed in the server side inside the client box.
                             -- ALso the server can quit by pressing the shutdown button on the server side
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

#define SOCKETNAME "/tmp/a2-27502333.socket"

int main(int argc, char *argv[]){
    

    // add background color to iol terminal
    printf("<program.push                           \n"
            "       background-color = x00FFFF      \n"
            ">                                      \n"
          );
    
    //add shutdown button to terminal
    printf("<button {Shutdown} color = x00FF00 background-color = {red} onclick=<putln {Q}>></n>\n");
    
    char buffer[1024];
    int n, sock, nsock, len;
    struct sockaddr_un name;

    //create a socket
    
    if((sock = socket(AF_UNIX, SOCK_STREAM,0)) < 0){
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
    
    //create a client box
   printf(
            "<client:box                    \n"
            "    background-color = xFFFFFF \n"
            "    width = 100,{%}            \n"
            "    visible = true             \n"
            ">                              \n"
    );
    
    //receive a message from client
    recv(nsock,buffer,sizeof(buffer),0);

    //push client message to client box
    printf(
            "<client.push <box              \n"
            "    color = xFFFFFF            \n"
            "    background-color = {blue}  \n"
            "    {%s}                       \n"
            "    width = 100,{%}            \n"
            "    visible = true             \n"
            ">>                             \n"
    ,buffer);
    
    //clear buffer
    memset(&buffer, 0, sizeof(buffer));

    //create a set of resources
    fd_set set_of_resources;
    
    int i = 1;
    char q = 'S';
    
    //while server does not quite, program keeps running
    while(q != 'Q'){
        
        //clear the buffer
        memset(&buffer, 0, sizeof(buffer));

        FD_ZERO(&set_of_resources);         //initialise set to 0
        FD_SET(nsock,&set_of_resources);    //monitor client connection
        FD_SET(0,&set_of_resources);        //monitor stdin
        
        //select resources to monitor
        select(nsock + 1,&set_of_resources,NULL,NULL,NULL);
        
        //check if state has changed of stdin
        if(FD_ISSET(0,&set_of_resources)){
           scanf("%c",q);     
        }
        
        //check if state has changed for client socket
        else if(FD_ISSET(nsock,&set_of_resources)){
            
            //receive from socket
            if(n=recv(nsock,buffer,sizeof(buffer),0) > 0){
                strcat(buffer,"\0");
                
                //add client message to client box
                printf(
                "<client.push                   \n"
                "    color = {red}              \n"
                "    {%s</n>}                   \n"
                ">                              \n"
                ,buffer);
            }
        }
    
    }

    //close socket connections
    close(nsock);
    close(sock);
    exit(0);
}
