

#include "P2P_client.h"

//#define SERVER_IP "192.168.1.8"
//#define SERVER_IP "104.39.105.56"
//#define SERVER_IP "128.118.36.57"
#define SERVER_HOSTNAME "Server1"

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include "../shared/P2P_shared.h"
#define PORT 8080 
#define SERVER_HOSTNAME "Server1"
   
#define BUFF_SIZE 1024


int main(int argc, char const *argv[]) 
{ 

    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[BUFF_SIZE] = {0}; 
    
    //Get host IP from the hostname
    char* hostname = SERVER_HOSTNAME;
    char ip[100];
    if(host_lookup(hostname,ip) < 0){
        perror("Error in host lookup");
        exit(EXIT_FAILURE);
    }


    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    send(sock , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 
    return 0; 
} 