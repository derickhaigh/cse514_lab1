#include "P2P_shared.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
//#include <sys/socket.h>
//#include <cstdint>
#include <arpa/inet.h>

//Lookup the host and get teh first IP available
int host_lookup( char* hostname, char* ip ){
    struct hostent *host;
    struct in_addr *addr;
    if( (host=gethostbyname( hostname )) == NULL){
        perror("Error getting host by name: ");
        return -1;
    }

    addr=(struct in_addr *) host->h_addr_list[0];
    if(addr == NULL){
        perror("Address list empty");
        return -1;
    }

    strcpy(ip, inet_ntoa(*addr));

    return 0;
}
