#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#include <search.h>
#include <dirent.h>
#include <map>
#include <string>

int send_message(char* target_host, uint16_t port, char* message, uint32_t message_len);

int menu_prompt();

void iterate_dir(DIR *p_dir, std::map<std::string,uint32_t> *file_registry, std::string root_dir);