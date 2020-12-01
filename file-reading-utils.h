#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

char *get_by_ref(int);
void get_by_keyword(char **, char *);

#endif