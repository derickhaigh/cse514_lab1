#include <search.h>
#include <stdint.h>
#include <map>
#include <vector>
#include <string>
int host_lookup(char* hostname, char* ip);

enum MESSAGE_TYPE {
    REGISTER=0,
    FILE_LIST=1,
    FILE_LOCATIONS=2,
    CHUNK_REGISTER=3,
    FILE_CHUNK=4,
};

/*struct file_descriptor{
    char filename[256];
    uint32_t file_len;
};*/

struct register_request{
    uint32_t requester_ip;
    uint16_t requester_port;

    uint16_t num_files;
    std::map<std::string,uint32_t> files_lengths;

};

struct register_reply {
    uint16_t num_files_registered;
    entry** files_registerd;    
};

struct file_entry {
    uint16_t file_size;
    uint16_t num_chunks;

    //Probably want to change to use IPs
    std::map<std::string,std::vector<uint16_t>> host_chunks;
};