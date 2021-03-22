#include <search.h>
#include <stdint.h>
int host_lookup(char* hostname, char* ip);

enum MESSAGE_TYPE {
    REGISTER=0,
    FILE_LIST=1,
    FILE_LOCATIONS=2,
    CHUNK_REGISTER=3,
    FILE_CHUNK=4,
};

struct file_descriptor{
    char filename[256];
    uint32_t file_len;
};

struct register_request{
    char requester_hostname[100];
    uint16_t requester_port;

    uint16_t num_files;

    /*
    entry{
        char *key;
        void *data;
    }
    */
    entry** files_lengths;

};

struct register_reply {
    uint16_t num_files_registered;
    entry** files_registerd;    
};