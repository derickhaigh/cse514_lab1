#include <map>
#include <vector>
#include <string>
#include <cstdint>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include "../shared/P2P_shared.h"

#define CHUNK_SIZE = 1048576  //1MB in bytes
#define PORT 8080
#define MAX_CONNECTIONS 32
#define MAX_PENDING_CONNECTIONS 16
#define MAX_THREADS 32






struct file_list_request {

};

struct file_list_reply {
    uint16_t num_files;
    std::map<std::string,uint32_t> files_lengths;
};

struct file_location_request{
    std::string file;
};

struct endpoint{
    uint16_t port;
    std::vector<uint32_t> chunk_ids;
};

struct file_location_reply{
    uint16_t num_endpoints;
    //IP, struct containting port and vector of chunk ids
    std::map<uint32_t,endpoint> endpoints;
};

struct chunk_register_request{
    std::string file;
    uint32_t chunk_id;
};

struct chuck_register_reply{
    bool file_register;
};

struct file_chunk_request{
    std::string file;
    uint32_t chunk_id;
};

struct file_chunk_reply{
    void* chunk_buffer;
};

int requestHandler(int client_fd);

int parse_request(int fd,char** req_buff, std::map<std::string,file_entry>& file_entries);

int register_files(int fd,char** req_buff, std::map<std::string,file_entry>& file_entries);

int file_list();

int file_locations();

int chunk_register();

int leave();