

#include "P2P_client.h"

//#define SERVER_IP "192.168.1.8"
//#define SERVER_IP "104.39.105.56"
//#define SERVER_IP "128.118.36.57"
#define SERVER_HOSTNAME "Server1"

#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h> 
#include <sys/stat.h>
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include "../shared/P2P_shared.h"
#include <dirent.h> // for reading direcories
#include <iostream>
#define PORT 8080 
#define SERVER_HOSTNAME "Server1"
   
#define BUFF_SIZE 1024
#define MAX_FILES 512
#define DEBUG true

int main(int argc, char const *argv[]) 
{ 
    //Check if the arguements are included
    if(argc < 4){
        fprintf(stderr,"Usage: %s server_hostname port {data_dir | file1,file2,file3,...}\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    //Get the server hostname
    char server_host[100];
    strcpy(server_host,argv[1]);

    //Get the destination port
    uint16_t port = atoi(argv[2]);

    //Get the field of files to register
    char* files=(char*) malloc(sizeof(char) * (strlen(argv[3])+1) );
    strcpy(files,argv[3]);
    //Construct a register message
    //Determine if we have a directory or a list of files
    //If we can open it as a directory it is, otherwise if is
    //either a list of files or invalid
    struct dirent *p_dirent;
    DIR *p_dir;

    std::map<std::string,uint32_t> file_registry;

    if((p_dir=opendir(files)) != NULL){
        //We have a directory, use a recursive function to add entries to function
        iterate_dir(p_dir,&file_registry,files);
    
    }else{
        //We might have a list of files, iterate through , seperated list and try to get file name and lengths
    }


    closedir(p_dir);
    free(files);


    //Debug, iterate through hash table for file entries
    std::map<std::string,uint32_t>::iterator itr;
    for(itr = file_registry.begin(); itr != file_registry.end(); itr++){
        std::cout<<itr->first<<": "<<itr->second<<std::endl;        
    }

    //Have the file registery, send the message
    std::cout<<sizeof(file_registry)<<std::endl;
    register_request reg_req;
    inet_pton(AF_INET,"127.0.0.1",&reg_req.requester_ip); //need to get actual IP
    reg_req.requester_port=8080;
    reg_req.num_files=file_registry.size();
    reg_req.files_lengths = file_registry;
    std::cout<<sizeof(reg_req)<<std::endl;
    std::cout<<sizeof(reg_req.files_lengths)<<std::endl;
    std::cout<<sizeof(std::string)<<std::endl;
    std::cout<<sizeof(uint32_t)<<std::endl;
    std::cout<<sizeof(reg_req.num_files)<<std::endl;
    std::cout<<sizeof(reg_req.requester_ip)<<std::endl;
    std::cout<<sizeof(reg_req.requester_port)<<std::endl;
    std::cout<<sizeof(std::pair<std::string,uint32_t>)<<std::endl;
    std::cout<<sizeof(std::pair<std::string,uint32_t>)*reg_req.files_lengths.size()<<std::endl;
    
    //Create a buffer to hold message
    //  message_type | requester_ip | requester_port | num_files | array of pairs from the map
    void* reg_buff = malloc(sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint16_t)+sizeof(uint16_t)+sizeof(std::pair<std::string,uint32_t>)*reg_req.files_lengths.size());
    void* curr_entry = reg_buff;
    
    //Set the message type in the first chunk of the buffer
    *((uint8_t*) curr_entry)=REGISTER;
    curr_entry=&(((uint8_t*) curr_entry)[1]);

    //Set requester IP
    *((uint32_t*) curr_entry)=reg_req.requester_ip;
    curr_entry=&(((uint32_t*) curr_entry)[1]);

    //Set requester port
    *((uint16_t*) curr_entry)=reg_req.requester_port;
    curr_entry=&(((uint16_t*) curr_entry)[1]);

    //Set number of files
    *((uint16_t*) curr_entry)=reg_req.num_files;
    curr_entry=&(((uint16_t*) curr_entry)[1]);

    //Start placing the file name/size pairs
    for(itr = file_registry.begin(); itr != file_registry.end(); itr++){
        std::cout<<itr->first<<": "<<itr->second<<std::endl;        
        ((std::string*) curr_entry)[0]=itr->first;
        curr_entry=&(((std::string*) curr_entry)[1]);

        *((uint32_t*) curr_entry)=itr->second;
        curr_entry=&(((uint32_t*) curr_entry)[1]);         
    }


    //Set a point right after the message type marker
/*    void* reg_buff_msg = &(((uint8_t*) reg_buff)[1]);
    ((register_request *) reg_buff_msg)[0].num_files=reg_req.num_files;
    ((register_request *) reg_buff_msg)[0].requester_ip=reg_req.requester_ip;
    ((register_request *) reg_buff_msg)[0].requester_port=reg_req.requester_port;
    ((register_request *) reg_buff_msg)[0].files_lengths=reg_req.files_lengths;
*/
    send_message("Server1",8080,(char*)reg_buff);

    //Begin the menu loop
    int choice;

    while(true){
        choice=menu_prompt();
        send_message(SERVER_HOSTNAME,PORT,"Test message");
    }
    return 0; 

} 

int menu_prompt(){
    int choice = 0;
    while(choice < 1 || choice > 3){
        printf("1) List Available Files\n2) Download File\n3) View Active Downloads\n");
        scanf("%d",&choice);
        printf("\n\n");
    }
    return choice;
}

int send_message(char* target_host, uint16_t port, char* message){
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

    char local_hostname[BUFF_SIZE];
    gethostname(local_hostname,BUFF_SIZE-1);


    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
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
    send(sock , local_hostname, strlen(local_hostname) , 0 ); 
    printf("Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 
    return 0;
}

void iterate_dir(DIR *p_dir, std::map<std::string,uint32_t> *file_registry, std::string root_dir){
    struct dirent *p_dirent;

    //Iterate through the directory entries in the current directory
    while ((p_dirent = readdir(p_dir)) != NULL) {
        DIR *p_subdir;
        struct stat st;

        //Add a / to the directory path just in case
        if(root_dir.back() != '/'){
            root_dir+='/';
        }
        std::string full_path = root_dir+p_dirent->d_name;
        const char* dir = full_path.c_str();
        std::cout<<full_path<<std::endl;
        if((p_subdir=opendir(dir)) != NULL){
            //Don't repeat directories, trying to exclude '.' and '..' and hidden directories like .ssh
	    //as it seems unlikely a user would want to share out their ssh keys
            if(p_dirent->d_name[0] != '.'){
                //We have a directory, use a recursive function to add entries to function
                iterate_dir(p_subdir, file_registry, root_dir+p_dirent->d_name);
                closedir(p_subdir);
            }
        }else{
            //We have a file, create an entry and hash it
            stat(dir,&st);

            file_registry->insert(std::pair<std::string,uint32_t>(p_dirent->d_name,st.st_size));
            if(DEBUG){
                printf("%s : %d \n",p_dirent->d_name,st.st_size);
            }
        }    
    }    


}
