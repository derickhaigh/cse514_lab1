#include <arpa/inet.h>
#include "P2P_server.h"
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../shared/P2P_shared.h"

#define PORT 8080
#define BUFF_SIZE 1024

int main(int argc, char* argv[]){

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt =1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server at ";

    //Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM,0)) == 0){
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    //Set socket options
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsocketopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //Bind socket to port
    if(bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0){
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    while(true){
        //Start to listen
        if(listen(server_fd, 3)<0){
            perror("Listen failed");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
        valread=read(new_socket,buffer,1024);
        printf("%s\n",buffer);
        send(new_socket,hello,strlen(hello),0);
        printf("Hello Message Sent\n");
    }


    return 0;

}

int requestHandler(int client_fd){
    char buffer[2000];
    int num_bytes;
    printf("Request Handler called for fd %i\n",client_fd);
    if(num_bytes=recv(client_fd,buffer,2000,0)<0){
        perror("Failure to read socket message");
        return -1;
    }else{
        printf("Message received from client_fd %i: %s\n",client_fd,buffer);
        send(client_fd,buffer,num_bytes,0);
        return 0;
    }

    

}