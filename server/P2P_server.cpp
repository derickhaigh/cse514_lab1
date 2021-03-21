#include <arpa/inet.h>
#include "P2P_server.h"
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BUFF_SIZE 1024

int requestHandler(int client_fd);

int main(int argc, char* argv[]){

    //Stuff for getaddrinfo
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUFF_SIZE];


    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt =1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";


    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(NULL, argv[1], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }


    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  /* Success */

        close(sfd);
    }

    freeaddrinfo(result);           /* No longer needed */

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

/*
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
    }*/

    //Start to listen
    if(listen(sfd, 3)<0){
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(sfd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    valread=read(new_socket,buffer,1024);
    printf("%s\n",buffer);
    send(new_socket,hello,strlen(hello),0);
    printf("Hello Message Sent");
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