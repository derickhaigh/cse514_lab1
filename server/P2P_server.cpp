#include <arpa/inet.h>
#include "P2P_server.h"
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../shared/P2P_shared.h"
#include <sys/epoll.h>


#define PORT 8080
#define BUFF_SIZE 4024
#define EPOLL_QUEUE_LEN 32
#define TIMEOUT 30
int main(int argc, char* argv[]){

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt =1;
    int addrlen = sizeof(address);
    char buffer[BUFF_SIZE] = {0};
    char *hello = "Hello from server at ";


    //Create epoll call FD
    uint16_t epfd = epoll_create1(0);

    if(epfd == -1){
        fprintf(stderr, "Creating epoll fd failed\n");
        exit(EXIT_FAILURE);
    }

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

    //Start to listen
    if(listen(server_fd, 3)<0){
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    //Register socket fd to the epoll
    struct epoll_event event;    
    //Set some flags on events to detect, read avail, connection close, and edge-trigger notification  
    event.events = EPOLLIN|EPOLLRDHUP|EPOLLET;
    event.data.fd = server_fd;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &event) == -1){
        perror("Error in epoll_ctl");
        exit(EXIT_FAILURE);
    }


    while(true){

        //Start monitoring epoll for connections
        struct epoll_event events[EPOLL_QUEUE_LEN];
        int n = epoll_wait(epfd,events,EPOLL_QUEUE_LEN,TIMEOUT);

        //Go through all events
        while(n-- > 0){
            if(events[n].events & EPOLLRDHUP){
                printf("Client socket closed\n");
                //If something needed to be done here do it
            }else if(events[n].events & EPOLLIN){
                if(events[n].data.fd == server_fd) {
                    printf("Connection accept\n");
                    
                    //Accept connection
                    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
                        perror("Accept failed");
                        exit(EXIT_FAILURE);
                    }     
                    //Register FD
                    struct epoll_event new_ev;
                    new_ev.events = EPOLLIN|EPOLLRDHUP|EPOLLET;
                    new_ev.data.fd=new_socket;
                    if(epoll_ctl(epfd, EPOLL_CTL_ADD, new_socket, &new_ev) == -1){
                        perror("Error in epoll_ctl");
                        exit(EXIT_FAILURE);
                    }                    

                }else{
                    char* read_buff = (char*)malloc(BUFF_SIZE);
                    printf("Get Data\n");
                    valread=read(events[n].data.fd,read_buff,BUFF_SIZE);
                    printf("%s\n",read_buff);
                    
                    //Seems like the stack is being stomped when calling?
                    parse_request(events[n].data.fd,&read_buff);  

                    /*//First byte holds a request type
                    char req_str[1];
                    strncpy(req_str,read_buff,1);
                    uint8_t request_type;
                    sscanf(req_str, "%d", &request_type);


                    switch(request_type){
                        case REGISTER:
                            register_files();
                            break;
                        case FILE_LIST:
                            file_list();
                            break;
                        case FILE_LOCATIONS:
                            file_locations();
                            break;
                        case CHUNK_REGISTER:
                            chunk_register();
                            break;
                        case FILE_CHUNK:
                            file_chunk();
                            break;
                    }*/


                    
                    send(events[n].data.fd,hello,strlen(hello),0);
                    printf("Hello Message Sent\n");     



                }
            }
        }



    }


    //close epoll
    if(close(epfd)){
        fprintf(stderr,"Failed to close epoll fd\n");
        exit(EXIT_FAILURE);
    }

    return 0;

}


int requestHandler(int client_fd){
    char buffer[2000];
    int num_bytes;
    printf("Request Handler called for fd %i\n",client_fd);
    if(num_bytes=recv(client_fd,buffer,2000,0)<0){
        perror("Failure to read socket message\n");
        return -1;
    }else{
        printf("Message received from client_fd %i: %s\n",client_fd,buffer);
        send(client_fd,buffer,num_bytes,0);
        return 0;
    }

    

}

int parse_request(int fd,char** req_buff){

    //First byte holds a request type
    char req_str[1];
    strncpy(req_str,*req_buff,1);
    uint8_t request_type=atoi(req_str);
    //sscanf(req_str, "%d", &request_type);
    


    switch(request_type){
        case REGISTER:
            register_files();
            break;
        case FILE_LIST:
            file_list();
            break;
        case FILE_LOCATIONS:
            file_locations();
            break;
        case CHUNK_REGISTER:
            chunk_register();
            break;
        case FILE_CHUNK:
            file_chunk();
            break;
    }


    return 0;
}

int register_files(){
    printf("register files\n");
    return 0;
}

int file_list(){
    printf("file_list\n");
    return 0;
}

int file_locations(){
    printf("file_locations\n");
    return 0;
}

int chunk_register(){
    printf("chunk_register\n");    
    return 0;
}

int file_chunk(){
    printf("file_chunk\n");    
    return 0;
}