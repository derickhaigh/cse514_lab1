#include <arpa/inet.h>
#include "P2P_server.h"
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>



int requestHandler(int client_fd);

int main(int argc, char* argv[]){

  // Initialize server socket
  //
    int server_options = 1;
    int server_fd, client_fd[MAX_CONNECTIONS]={0};
    socklen_t server_addrlen;
    struct sockaddr_in server_address, client_address;
    fd_set read_fd_set,server_fd_set;
    int select_activity;
    int largest_fd;

    char buffer[1025];

    //Create server socket fd and set it to allow multiple connections
    if((server_fd=socket(AF_INET, SOCK_STREAM,0))==0){
        perror("Failure establishing socket");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (void*)&server_options,sizeof(server_options))<0){
        perror("Failure setting socket options");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=INADDR_ANY;
    server_address.sin_port=htons(PORT);
    memset(&(server_address.sin_zero),'\0',8);

    //Bind the socket to an address and port
    if(bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address))<0){
        perror("Failure binding server socket");
        exit(EXIT_FAILURE);
    }
    
    //Listen for connection to be established
    if(listen(server_fd,MAX_PENDING_CONNECTIONS)<0){
        perror("Failure to listen on server socket");
        exit(EXIT_FAILURE);
    }
    
    FD_ZERO(&server_fd_set);
    FD_ZERO(&read_fd_set);
    FD_SET(server_fd,&server_fd_set);
     
    largest_fd = server_fd;

    while(true){


        //block until socket activity arrives
        FD_ZERO(&read_fd_set);
        read_fd_set=server_fd_set;

/*        for(int i=0;i<MAX_CONNECTIONS;i++){
            int sd = client_fd[i];
            if(sd){
                FD_SET(sd,&read_fd_set);
            }
        }
  */      
        printf("Block for select\n");
        if(select_activity=select(FD_SETSIZE,&read_fd_set,NULL,NULL,NULL)<0){
            perror("Failure during select()");
            exit(EXIT_FAILURE);
        }
        printf("Avtivity count: %i\n",select_activity);
/*        if(FD_ISSET(server_fd,&server_fd_set)){
            int fd;
            if((fd=accept(server_fd,(struct sockaddr *)&server_address,(socklen_t*)&server_addrlen))<0){
                perror("Failure accepting connection");
            }
            //Add new socket to array
            for(int i=0;i<FD_SETSIZE;i++){
                if(client_fd[i]==0){
                    client_fd[i]=fd;
                    break;
                }
            }
            //FD_SET(fd,&read_fd_set);
            if(fd>largest_fd){
                largest_fd=fd;
            }
            printf("New connection at fd: %i\n",fd);
            
        }else{
            for(int i;i<largest_fd;i++){
                int fd = client_fd[i];
                int val;
                if(FD_ISSET(fd,&read_fd_set)){
                    //Check if closing
                    if((val=read(fd,buffer,1024))<=0){
                        //getpeername(fd,(struct sockaddr*)server_address,(socklen_t)&server_addrlen);
                        close(fd);
                        client_fd[i]=0;
                        printf("Closing fd: %i\n",fd);
                    }else{
                        send(fd,"Message recieved\0",strlen(buffer),0);
                    }
                }
            }
        }
            
    } */
       printf("Loop through connections\n"); 
       for(int i = 0; i<MAX_CONNECTIONS;i++){
            printf("Check FD %i\n",i);
            if(FD_ISSET(i,&read_fd_set))
            {
                printf("Is set\n");
                //Request on server socket, must be a connection
                if(i==server_fd){
                    int fd;
                    server_addrlen = sizeof server_address;
                    if(fd=accept(server_fd,(struct sockaddr*)&server_address, &server_addrlen)<0){
                        perror("Failure to accept client");
                        continue;
                    }
                    FD_SET(fd,&server_fd_set);
                    printf("New connection with fd: %i\n",fd);
                    fprintf(stderr,
                         "Server: connect from host %s, port %i.\n",
                         inet_ntoa (server_address.sin_addr),
                         ntohs (server_address.sin_port));

                    char buffer[2000];
                    if(recv(fd,buffer,2000,0)<0){
                       perror("Failure to read"); 
                    }

                    printf("Recieved message from fd %i\n",fd);

                    send(fd,buffer,2000,0);
                    printf("Response sent to fd %i\n",fd);

                }else{ 
                    //Data arriving on pre-existing socket
                    printf("Data arrived\n");
                    if(requestHandler(i)<0){
                        close(i);
                        FD_CLR(i,&server_fd_set);
                        printf("Closing connection with fd: %i\n",i);
                    } 
                }
            }
        }

      
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