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
int send_message(char* target_host, uint16_t port, char* message);

int menu_prompt();

int iterate_dir(DIR *p_dir);