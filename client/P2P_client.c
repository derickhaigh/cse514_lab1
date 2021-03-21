#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>

//#define SERVER_IP "192.168.1.8"
//#define SERVER_IP "104.39.105.56"
#define SERVER_IP "128.118.36.57"

int main()
{
  char message[1000];
  char buffer[1024];
  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  int client_options=1;

  // Create the socket. 
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  //Configure settings of the server address
    if(setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &client_options, sizeof(client_options))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
 // Address family is Internet 
  serverAddr.sin_family = AF_INET;

  //Set port number, using htons function 
  serverAddr.sin_port = htons(8080);

 //Set IP address to localhost
  serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    //Connect the socket to the server using the address
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
    strcpy(message,"Hello");
    if( send(clientSocket , message , strlen(message) , 0) < 0)
    {
            printf("Send failed\n");
    }

    //Read the message from the server into the buffer
    if(recv(clientSocket, buffer, 1024, 0) < 0)
    {
       printf("Receive failed\n");
    }

    //Print the received message
    printf("Data received: %s\n",buffer);

    int hellos=0;
    //Proceed to send more data and read responses
    for(int i=0;i<50;i++){
        strcpy(message,"Hello again!");
        printf("Sending Message");
        if( send(clientSocket , message , strlen(message) , 0) < 0)
        {
            printf("Send failed\n");
        }

        //Read the message from the server into the buffer
        if(recv(clientSocket, buffer, 1024, 0) < 0)
        {
            printf("Receive failed\n");
        }

        //Print the received message
        printf("Data received: %s\n",buffer);   
        sleep(2);
    }

    close(clientSocket);

}

