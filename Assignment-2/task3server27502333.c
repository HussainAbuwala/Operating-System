/*
    Name: HUSSAIN SADIQ ABUWALA
    STUDENT-ID: 27502333
    HIGH-LEVEL-DESCRIPTION - -- The client can send messages to the server and the messages will be displayed in the server side inside the client box.
                             -- ALso the server can quit by pressing the shutdown button on the server side
                             -- Up to 5 clients can connect to the server and send messages

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

void setFD_Clients(int clients[5], fd_set *set_of_resources,int total_elems);

int main(int argc, char *argv[]){
    
    //add background color to iol terminal
    printf("<program.push                           \n"
            "       background-color=x092D63      \n"
            "       color = {blue}                  \n"
            ">                                      \n"
          );
    
    //add shutdown button to terminal
    printf("<button {Shutdown} size=1.7,{em} border-color=x092D63 color=x092D63 background-color={white} onclick=<putln {Q}>></n>\n");
    printf("</n>\n");
    printf("</n>\n");
    char buffer[1024];
    int n, sock, nsock, len;
    int clients[5];
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
    
    
    // create the set of resources to monitor
    fd_set set_of_resources;
    
    char q = 'S';
    int total_clients = 0;
    
    //keep running till shutdown button is pressed on server
    while(q != 'Q'){
        // clear buffer
        memset(&buffer, 0, sizeof(buffer));

        FD_ZERO(&set_of_resources);                                 //initialise set to 0

        FD_SET(sock,&set_of_resources);                             //monitor listen connection from client
        FD_SET(0,&set_of_resources);                                //monitor stdin
        
        //if clients connected, monitor clients input
        if(total_clients > 0){
            setFD_Clients(clients,&set_of_resources,total_clients);     //monitor client input
            select(clients[total_clients - 1] + 1,&set_of_resources,NULL,NULL,NULL);
        }
        //if no clients, monitor listen and stdin
        if(total_clients == 0){
            select(sock + 1,&set_of_resources,NULL,NULL,NULL);
        }
        //check if state changed of stdin
        if(FD_ISSET(0,&set_of_resources)){
           scanf("%c\n",q);     
        }
        //check if any new client connections incoming
        else if(FD_ISSET(sock,&set_of_resources) && total_clients <= 4){
            
            //if total clients are 5, dont accept any other client connections
            

                listen(sock, 5);

                nsock = accept(sock, (struct sockaddr *) &name, &len);
                clients[total_clients] = nsock;
                
                char string1[20] = "client";
                char s[] = {'0' + total_clients,'\0'};
                strcat(string1,s);


                //add client header box
                printf(
                        "<%s:box                    \n"
                        "    background-color=x2CE28C \n"
                        "    border-color=x2CE28C\n"
                        "    border = 8\n"
                        "    width = 100,{%}            \n"
                        "    visible = true             \n"
                        "    size = 1.6,{em}            \n"
                        ">                              \n"
                ,string1);
                
                //receive message from client
                recv(nsock,buffer,sizeof(buffer),0);
                
                //add message to client specific box
                printf(
                        "<%s.push <box              \n"
                        "    color = xFFFFFF            \n"
                        "    background-color = x1162BC  \n"
                        "    border-color=x1162BC        \n"
                        "    border = 2\n"
                        "    {%s}                       \n"
                        "    width = 70,{%}            \n"
                        "    visible = true             \n"
                        "    size = 1.7,{em}            \n"
                        "> {</n>}>                             \n"
                ,string1,buffer);

                printf("</n>\n");
                printf("</n>\n");
                
                //increase count of total clients
                total_clients = total_clients + 1;
            
                     
        }
        
        else{

            int j;
            for (j = 0; j < total_clients; j++){
                //check if any client has sent any new message
                if(FD_ISSET(clients[j],&set_of_resources)){

                    //receive from client
                    if(n=recv(clients[j],buffer,sizeof(buffer),0) > 0){;
                        strcat(buffer,"\0");
                        char string1[20] = "client";
                        char s[] = {'0' + j,'\0'};
                        strcat(string1,s);

                        //add message to client specfic box
                        printf(
                                    "<%s.push             \n"
                                    "    color=x1162BC    \n"
                                    "    {%s</n>}         \n"
                                    ">                    \n"
                              ,string1,buffer);
                        break;
                    }
                }
            }
        
        }
        
        
    
    }

 
    //close all connections
    close(nsock);
    close(sock);
    exit(0);
}


void setFD_Clients(int clients[5], fd_set *set_of_resources,int total_elems){
    
    int i;  
    for(i = 0; i < total_elems; i++){
        FD_SET(clients[i],set_of_resources);
    }  

}



